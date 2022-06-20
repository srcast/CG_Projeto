#include "generator/primitives/plane.hpp"

#include "util/try.hpp"

#include <new>
#include <stdexcept>

auto normals_plane = std::vector<glm::vec3>{};
auto texcoord_plane = std::vector<glm::vec2>{};

namespace generator {

using namespace brief_int;

auto generate_plane(float const side_len, u32 const num_divs) noexcept
    -> cpp::result<std::vector<glm::vec3>, GeneratorErr>
try {
    using namespace brief_int::literals;

    if (num_divs == 0) {
        // A plane cannot have zero divisions, otherwise the expression used to
        // calculate the division side length (side_len / num_divs) will fail.
        return cpp::fail(GeneratorErr::PLANE_ZERO_DIVS);
    }

    // We cache num_divs as a usize because we're going to use it in the next
    // expression.
    auto const num_divs_uz = static_cast<usize>(num_divs);

    // The total amount of vertices the cone will contain.
    auto const total_vertex_count
        = num_divs_uz * num_divs_uz // number of divisions in the plane.
        * 2                         // number of triangles in a division.
        * 3;                        // number of vertices in a triangle.

    // We push every vertex to this vector.
    // At the end of the function call, it must contain total_vertex_count
    // vertices.
    // NOTE:
    //   * These vertices are stored using a CARTESIAN coordinate system
    //     (https://en.wikipedia.org/wiki/Cartesian_coordinate_system),
    //     where:
    //       - the first coordinate is x;
    //       - the second coordinate is always zero (the plane is on the xOz
    //         plane);
    //       - the third coordinate is z.
    //   * The generated triangles follow the CCW (counter-clockwise)
    //     convention.
    auto vertices = std::vector<glm::vec3>{};
    vertices.reserve(total_vertex_count);
    normals_plane.reserve(total_vertex_count);
    texcoord_plane.reserve(total_vertex_count);

    // Stores the side length of a plane division.
    auto const div_side_len = side_len / static_cast<float>(num_divs);

    auto const div_text_coord = 1 / static_cast<float>(num_divs);

    // We iterate division by division.
    // The outer loop traverses the z axis and the inner the loop traverses the
    // x axis.
    for (auto row = 0_u32; row < num_divs; ++row) {
        // Stores the lower (with lower magnitude, not positionally below) z
        // coordinate of the current division.
        auto const lo_z = static_cast<float>(row) * div_side_len;
        auto const lo_y_text_coord = static_cast<float>(num_divs - row) * div_text_coord;

        // Stores the upper (with higher magnitude, not positionally above) z
        // coordinate of the current division.
        auto const hi_z = static_cast<float>(row + 1) * div_side_len;
        auto const hi_y_text_coord = static_cast<float>(num_divs - row - 1) * div_text_coord;

        for (auto col = 0_u32; col < num_divs; ++col) {
            // Stores the lower (with lower magnitude, not positionally below) x
            // coordinate of the current division.
            auto const lo_x = static_cast<float>(col) * div_side_len;
            auto const lo_x_text_coord = static_cast<float>(col) * div_text_coord;

            // Stores the upper (with higher magnitude, not positionally above)
            // x coordinate of the current division.
            auto const hi_x = static_cast<float>(col + 1) * div_side_len;
            auto const hi_x_text_coord = static_cast<float>(col + 1) * div_text_coord;

            // First we generate the first half of the division.
            vertices.emplace_back(lo_x, 0.f, lo_z);
            vertices.emplace_back(lo_x, 0.f, hi_z);
            vertices.emplace_back(hi_x, 0.f, lo_z);

            normals_plane.emplace_back(0.f, 1.f, 0.f);
            normals_plane.emplace_back(0.f, 1.f, 0.f);
            normals_plane.emplace_back(0.f, 1.f, 0.f);

            texcoord_plane.emplace_back(lo_x_text_coord, lo_y_text_coord);
            texcoord_plane.emplace_back(lo_x_text_coord, hi_y_text_coord);
            texcoord_plane.emplace_back(hi_x_text_coord, lo_y_text_coord);
            

            // Then we generate the second.
            vertices.emplace_back(hi_x, 0.f, lo_z);
            vertices.emplace_back(lo_x, 0.f, hi_z);
            vertices.emplace_back(hi_x, 0.f, hi_z);

            normals_plane.emplace_back(0.f, 1.f, 0.f);
            normals_plane.emplace_back(0.f, 1.f, 0.f);
            normals_plane.emplace_back(0.f, 1.f, 0.f);

            texcoord_plane.emplace_back(hi_x_text_coord, lo_y_text_coord);
            texcoord_plane.emplace_back(lo_x_text_coord, hi_y_text_coord);
            texcoord_plane.emplace_back(hi_x_text_coord, hi_y_text_coord);
        }
    }

    // Again, since the plane is on the xOz plane, we can hardcode the y
    // coordinate to zero.
    // Moreover, to ensure the plane is centered at the origin, we need to
    // perform a translation of negative half_side_len.
    for (
        // Stores half of the plane side length.
        auto const half_side_len = side_len / 2.f;
        auto&& vertex : vertices
    ) {
        vertex.x -= half_side_len;
        vertex.z -= half_side_len;
    }

    return vertices;

} catch (std::bad_alloc const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
} catch (std::length_error const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
}

auto generate_and_print_plane(
    float const side_len,
    u32 const num_divs,
    fmt::ostream& output_file
) noexcept -> cpp::result<void, GeneratorErr>
try {
    auto const& vertices = TRY_RESULT(generate_plane(side_len, num_divs));
    output_file.print("{}\n", vertices.size());
    for (auto const& vertex : vertices) {
        output_file.print("{} {} {}\n", vertex.x, vertex.y, vertex.z);
    }
    output_file.print("\n");
    for (auto const& vertex : normals_plane) {
        output_file.print("{} {} {}\n", vertex.x, vertex.y, vertex.z);
    }
    output_file.print("\n");
    for (auto const& vertex : texcoord_plane) {
        output_file.print("{} {}\n", vertex.x, vertex.y);
    }
    return {};
} catch (...) {
    return cpp::fail(GeneratorErr::IO_ERR);
}

} // namespace generator
