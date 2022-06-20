#include "generator/primitives/cone.hpp"

#include "util/coord_conv.hpp"
#include "util/try.hpp"

#include <glm/ext/scalar_constants.hpp>
#include <new>
#include <stdexcept>

auto normals_cone = std::vector<glm::vec3>{};
auto texcoord_cone = std::vector<glm::vec2>{};

namespace generator {

glm::vec3 cross(glm::vec3 a, glm::vec3 b) {
    glm::vec3 res;
    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
    return res;
}

glm::vec3 normalize(glm::vec3 a) {
    glm::vec3 res;
    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    res[0] = a[0]/l;
    res[1] = a[1]/l;
    res[2] = a[2]/l;
    return res;
}

using namespace brief_int;

auto generate_cone(
    float const radius,
    float const height,
    u32 const num_slices,
    u32 const num_stacks
) noexcept -> cpp::result<std::vector<glm::vec3>, GeneratorErr>
try {
    using namespace brief_int::literals;

    if (num_slices <= 2) {
        // A cone needs at least 3 slices to be properly generated.
        return cpp::fail(GeneratorErr::CONE_LT_THREE_SLICES);
    }

    if (num_stacks == 0) {
        // A cone cannot have zero stacks, otherwise the expressions used to
        // calculate the radius factor (radius / num_stacks) and stack height
        // (height / num_stacks) will fail.
        return cpp::fail(GeneratorErr::CONE_ZERO_STACKS);
    }

    // We cache num_stacks as a float because we're going to use it in some
    // expressions ahead.
    auto const num_stacks_f = static_cast<float>(num_stacks);

    // Stores the angle, in RADIANS, of a slice.
    // Radians are used instead of degrees to allow easy conversion from
    // cylindrical coordinates to cartesian coordinates, at the end of the
    // function.
    // This specific order of operations *should* provide the best
    // approximation, since 2.f and num_stacks_f are both integers.
    auto const slice_angle
        = glm::pi<float>()
        * (2.f / static_cast<float>(num_slices));

    // Stores a factor with which to compute the radius at a given stack
    // separator. This value is equivalent to the radius of the last (upper)
    // stack.
    auto const radius_factor = radius / num_stacks_f;

    // Stores the height of a stack.
    auto const stack_height = height / num_stacks_f;

    // We cache this value because we're going to use it as an iteration limit
    // in the inner loop.
    // This expression won't wrap around since num_stacks cannot be lower
    // than 1, which we have asserted already.
    auto const num_stacks_minus_one = num_stacks - 1;

    auto const texstack = 1.0f / static_cast<float>(num_stacks);
    auto const texslice = 1.0f / static_cast<float>(num_slices);

    // Stores the distance from the base of the cone to the last stack
    // separator.
    auto const top_height = height - stack_height;

    // The "pointy end" of the cone
    // (https://en.wikipedia.org/wiki/Apex_(geometry)).
    // We cache this value because we're going to use it a lot when generating
    // the last stack of the cone.
    auto const apex = glm::vec3{0.f, height, 0.f};

    // The total amount of vertices the cone will contain.
    auto const total_vertex_count
        = 6 // magic math number.
        * static_cast<usize>(num_slices)
        * static_cast<usize>(num_stacks);

    // We push every vertex to this vector.
    // At the end of the function call, it must contain total_vertex_count
    // vertices.
    // NOTE:
    //   * These vertices are stored using a CYLINDRICAL coordinate system
    //     (https://en.wikipedia.org/wiki/Cylindrical_coordinate_system),
    //     where:
    //       - the first coordinate is the radial distance aka radius;
    //       - the second coordinate is the altitude aka height;
    //       - the third coordinate is the azimuth aka angular position in
    //         RADIANS.
    //     Usually the second coordinate is the azimuth and the third coordinate
    //     is the altitude. However, in order to play nice with OpenGL, these
    //     coordinates have been swapped, since in its 3D space the second
    //     coordinate is the altitude.
    //     In order to forward these vertices to OpenGL, these coordinates need
    //     to be converted to a CARTESIAN coordinate system;
    //   * The generated triangles follow the CCW (counter-clockwise)
    //     convention.
    auto vertices = std::vector<glm::vec3>{};
    vertices.reserve(total_vertex_count);

    glm::vec3 n;

    // We iterate slice by slice, stack by stack.
    // i represents the current slice.
    for (auto i = 0_u32; i < num_slices; ++i) {

        auto const i_f = static_cast<float>(i);

        // Stores the accumulated angle, i.e. the sum of all angles from the
        // first slice up until this slice.
        auto const curr_angle = static_cast<float>(i) * slice_angle;

        // Same, but for the next slice.
        auto const next_angle = static_cast<float>(i + 1) * slice_angle;

        // The base of the cone is composed of num_slices triangles.
        // The next 3 lines of code generate the base of the current slice,
        // i.e. the i'th triangle that constitutes the base of the cone.
        //
        // First we generate the center of the base of the cone, which is
        // (conveniently) the point (0, 0, 0).
        vertices.emplace_back(0.f, 0.f, 0.f);
        vertices.emplace_back(radius, 0.f, next_angle);
        vertices.emplace_back(radius, 0.f, curr_angle);

        normals_cone.emplace_back(0.f, -1.f, 0.f);
        normals_cone.emplace_back(0.f, -1.f, 0.f);
        normals_cone.emplace_back(0.f, -1.f, 0.f);

        texcoord_cone.emplace_back(0.5f, 0.5f);
        texcoord_cone.emplace_back(0.5f + 0.5f * cos(next_angle), 0.5f + 0.5f * sin(next_angle));
        texcoord_cone.emplace_back(0.5f + 0.5f * cos(curr_angle), 0.5f + 0.5f * sin(curr_angle));

        // Next, we generate the "body" of the slice, that is, the "walls" of
        // the slice that constitute the "walls" of the cone.
        // We generate these vertices up until (excluding) num_stacks, since the
        // last stack must be drawn differently than the other stack.
        //
        // These first walls are represented by
        // quads (https://en.wikipedia.org/wiki/Quadrilateral), i.e.
        // 2 triangles joined, while the last (upper) walls of the cone are
        // simply triangles.
        
        glm::vec3 a = {radius,0,curr_angle};
        glm::vec3 b = {radius,0,next_angle};
        glm::vec3 c = {radius - radius_factor,stack_height,curr_angle};
        glm::vec3 res;

        ::util::cylindrical_to_cartesian_inplace(a);
        ::util::cylindrical_to_cartesian_inplace(b);
        ::util::cylindrical_to_cartesian_inplace(c);

        glm::vec3 vec1 = {b[0]-a[0],b[1]-a[1],b[2]-a[2]};
        glm::vec3 vec2 = {c[0]-a[0],c[1]-a[1],c[2]-a[2]};

        res = cross(vec1,vec2);

        n = normalize(res);
       
        // j represents the current stack.
        for (auto j = 0_u32; j < num_stacks_minus_one; ++j) {
            // We cache j as a float because we're going to use it more than
            // once.
            auto const j_f = static_cast<float>(j);

            // We do the same of j + 1.
            auto const j_plus_1_f = static_cast<float>(j + 1);

            // Stores the distance FROM the normal that intersects
            // the center of the base of the cone TO any vertex that constitutes
            // the current stack separator.
            auto const curr_radius = radius - j_f * radius_factor;

            // The same as the above, but for the next stack separator, where
            // 'the next stack' means 'the stack above this stack (in the
            // vertical y axis)'.
            auto const next_radius = radius - j_plus_1_f * radius_factor;

            // Stores the distance FROM the base of the cone TO any vertex that
            // constitutes the current stack separator.
            auto const curr_height = j_f * stack_height;

            // The same as the above, but for the next stack separator, where
            // 'the next stack' means 'the stack above this stack (in the
            // vertical y axis)'.
            auto const next_height = j_plus_1_f * stack_height;

            // First we generate the first half of the slice wall.
            vertices.emplace_back(next_radius, next_height, next_angle);
            vertices.emplace_back(next_radius, next_height, curr_angle);
            vertices.emplace_back(curr_radius, curr_height, next_angle);

            // Then we generate the second.
            vertices.emplace_back(curr_radius, curr_height, curr_angle);
            vertices.emplace_back(curr_radius, curr_height, next_angle);
            vertices.emplace_back(next_radius, next_height, curr_angle);

            normals_cone.emplace_back(n[0], n[1], n[2]);
            normals_cone.emplace_back(n[0], n[1], n[2]);
            normals_cone.emplace_back(n[0], n[1], n[2]);

            normals_cone.emplace_back(n[0], n[1], n[2]);
            normals_cone.emplace_back(n[0], n[1], n[2]);
            normals_cone.emplace_back(n[0], n[1], n[2]);

            texcoord_cone.emplace_back((i_f+1) * texslice, j_plus_1_f * texstack);
            texcoord_cone.emplace_back(i_f * texslice, j_plus_1_f * texstack);
            texcoord_cone.emplace_back((i_f+1) * texslice, j_f * texstack);

            texcoord_cone.emplace_back(i_f * texslice, j_f * texstack);
            texcoord_cone.emplace_back((i_f+1) * texslice, j_f * texstack);
            texcoord_cone.emplace_back(i_f * texslice, j_plus_1_f * texstack);
        }

        // Finally, we generate the upper wall of the slice.
        // Again, this is just a triangle, not a quad.
        // Reminder that radius_factor is equivalent to the radius of the last
        // (upper) stack.
        vertices.push_back(apex);
        vertices.emplace_back(radius_factor, top_height, curr_angle);
        vertices.emplace_back(radius_factor, top_height, next_angle);

        normals_cone.emplace_back(n[0], n[1], n[2]);
        normals_cone.emplace_back(n[0], n[1], n[2]);
        normals_cone.emplace_back(n[0], n[1], n[2]);

        texcoord_cone.emplace_back(i_f * texslice,1.0f);
        texcoord_cone.emplace_back(i_f * texslice, texstack*static_cast<float>(num_stacks_minus_one));
        texcoord_cone.emplace_back((i_f+1) * texslice, texstack*static_cast<float>(num_stacks_minus_one));
    }

    // We need to make sure the vertices are represented in a cartesian
    // coordinate system before we forward them to OpenGL.
    for (auto&& vertex : vertices) {
        ::util::cylindrical_to_cartesian_inplace(vertex);
    }

    return vertices;

} catch (std::bad_alloc const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
} catch (std::length_error const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
}

auto generate_and_print_cone(
    float const radius,
    float const height,
    u32 const num_slices,
    u32 const num_stacks,
    fmt::ostream& output_file
) noexcept -> cpp::result<void, GeneratorErr>
try {
    auto const& vertices = TRY_RESULT(
        generate_cone(radius, height, num_slices, num_stacks)
    );
    output_file.print("{}\n", vertices.size());
    for (auto const& vertex : vertices) {
        output_file.print("{} {} {}\n", vertex.x, vertex.y, vertex.z);
    }
    output_file.print("\n");
    for (auto const& vertex : normals_cone) {
        output_file.print("{} {} {}\n", vertex.x, vertex.y, vertex.z);
    }
    output_file.print("\n");
    for (auto const& vertex : texcoord_cone) {
        output_file.print("{} {}\n", vertex.x, vertex.y);
    }
    return {};
} catch (...) {
    return cpp::fail(GeneratorErr::IO_ERR);
}

} // namespace generator
