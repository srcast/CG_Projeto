#include "generator/primitives/box.hpp"

#include "util/try.hpp"

#include <new>
#include <stdexcept>

namespace generator {

using namespace brief_int;

auto generate_box(float const side_len, u32 const num_divs) noexcept
    -> cpp::result<std::vector<glm::vec3>, GeneratorErr>
try {
    using namespace brief_int::literals;

    if (num_divs == 0) {
        // A box cannot have zero divisions, otherwise the expression used to
        // calculate the division side length (side_len / num_divs) will fail.
        return cpp::fail(GeneratorErr::BOX_ZERO_DIVS);
    }

    // We cache num_divs as a usize because we're going to use it in the next
    // expression.
    auto const num_divs_uz = static_cast<usize>(num_divs);

    // The total amount of vertices the box will contain.
    auto const total_vertex_count
        = 6                         // number of faces in a cube.
        * num_divs_uz * num_divs_uz // number of divisions in a face.
        * 2                         // number of triangles in a division.
        * 3;                        // number of vertices in a triangle.

    // We push every vertex to this vector.
    // At the end of the function call, it must contain total_vertex_count
    // vertices.
    // NOTE:
    //   * The box is not centered at the origin. Instead, its bottom left back
    //     corner is placed on the origin. We want the box to be centered at the
    //     origin, so we need to perform a translation of -side_len/2 to each
    //     vertex before we forward them to OpenGL;
    //   * These vertices are stored using a CARTESIAN coordinate system
    //     (https://en.wikipedia.org/wiki/Cartesian_coordinate_system);
    //   * The generated triangles follow the CCW (counter-clockwise)
    //     convention.
    auto vertices = std::vector<glm::vec3>{};
    vertices.reserve(total_vertex_count);

    // Stores the side length of a box side division.
    auto const div_side_len = side_len / static_cast<float>(num_divs);

    // We generate the vertices for each plane manually.
    // The order in which the planes are generated is:
    // front -> back -> left -> right -> top -> bottom.
    // The front plane is the plane parallel to the plane xOy, with positive z.

    // Generate the front plane.
    // We iterate division by division.
    // The outer loop traverses the y axis and the inner the loop traverses the
    // x axis.
    for (auto row = 0_u32; row < num_divs; ++row) {
        // Stores the lower (with lower magnitude, not positionally below) y
        // coordinate of the current division.
        auto const lo_y = static_cast<float>(row) * div_side_len;

        // Stores the upper (with higher magnitude, not positionally above) y
        // coordinate of the current division.
        auto const hi_y = static_cast<float>(row + 1) * div_side_len;

        for (auto col = 0_u32; col < num_divs; ++col) {
            // Stores the lower (with lower magnitude, not positionally below) x
            // coordinate of the current division.
            auto const lo_x = static_cast<float>(col) * div_side_len;

            // Stores the upper (with higher magnitude, not positionally above)
            // x coordinate of the current division.
            auto const hi_x = static_cast<float>(col + 1) * div_side_len;

            // First we generate the first half of the division.
            vertices.emplace_back(lo_x, hi_y, side_len);
            vertices.emplace_back(lo_x, lo_y, side_len);
            vertices.emplace_back(hi_x, hi_y, side_len);

            // Then we generate the second.
            vertices.emplace_back(hi_x, hi_y, side_len);
            vertices.emplace_back(lo_x, lo_y, side_len);
            vertices.emplace_back(hi_x, lo_y, side_len);
        }
    }

    // Generate the back plane.
    // We iterate division by division.
    // The outer loop traverses the y axis and the inner the loop traverses the
    // x axis.
    for (auto row = 0_u32; row < num_divs; ++row) {
        // Stores the lower (with lower magnitude, not positionally below) y
        // coordinate of the current division.
        auto const lo_y = static_cast<float>(row) * div_side_len;

        // Stores the upper (with higher magnitude, not positionally above) y
        // coordinate of the current division.
        auto const hi_y = static_cast<float>(row + 1) * div_side_len;

        for (auto col = 0_u32; col < num_divs; ++col) {
            // Stores the lower (with lower magnitude, not positionally below) x
            // coordinate of the current division.
            auto const lo_x = static_cast<float>(col) * div_side_len;

            // Stores the upper (with higher magnitude, not positionally above)
            // x coordinate of the current division.
            auto const hi_x = static_cast<float>(col + 1) * div_side_len;

            // Since this plane is contained in the xOy plane, every vertex's
            // z coordinate is zero.

            // First we generate the first half of the division.
            vertices.emplace_back(lo_x, hi_y, 0.f);
            vertices.emplace_back(hi_x, lo_y, 0.f);
            vertices.emplace_back(lo_x, lo_y, 0.f);

            // Then we generate the second.
            vertices.emplace_back(hi_x, hi_y, 0.f);
            vertices.emplace_back(hi_x, lo_y, 0.f);
            vertices.emplace_back(lo_x, hi_y, 0.f);
        }
    }

    // Generate the left plane.
    // We iterate division by division.
    // The outer loop traverses the y axis and the inner the loop traverses the
    // z axis.
    for (auto row = 0_u32; row < num_divs; ++row) {
        // Stores the lower (with lower magnitude, not positionally below) y
        // coordinate of the current division.
        auto const lo_y = static_cast<float>(row) * div_side_len;

        // Stores the upper (with higher magnitude, not positionally above) y
        // coordinate of the current division.
        auto const hi_y = static_cast<float>(row + 1) * div_side_len;

        for (auto col = 0_u32; col < num_divs; ++col) {
            // Stores the lower (with lower magnitude, not positionally below) z
            // coordinate of the current division.
            auto const lo_z = static_cast<float>(col) * div_side_len;

            // Stores the upper (with higher magnitude, not positionally above)
            // z coordinate of the current division.
            auto const hi_z = static_cast<float>(col + 1) * div_side_len;

            // Since this plane is contained in the yOz plane, every vertex's
            // x coordinate is zero.

            // First we generate the first half of the division.
            vertices.emplace_back(0.f, hi_y, lo_z);
            vertices.emplace_back(0.f, lo_y, lo_z);
            vertices.emplace_back(0.f, hi_y, hi_z);

            // Then we generate the second.
            vertices.emplace_back(0.f, hi_y, hi_z);
            vertices.emplace_back(0.f, lo_y, lo_z);
            vertices.emplace_back(0.f, lo_y, hi_z);
        }
    }

    // Generate the right plane.
    // We iterate division by division.
    // The outer loop traverses the y axis and the inner the loop traverses the
    // z axis.
    for (auto row = 0_u32; row < num_divs; ++row) {
        // Stores the lower (with lower magnitude, not positionally below) y
        // coordinate of the current division.
        auto const lo_y = static_cast<float>(row) * div_side_len;

        // Stores the upper (with higher magnitude, not positionally above) y
        // coordinate of the current division.
        auto const hi_y = static_cast<float>(row + 1) * div_side_len;

        for (auto col = 0_u32; col < num_divs; ++col) {
            // Stores the lower (with lower magnitude, not positionally below) z
            // coordinate of the current division.
            auto const lo_z = static_cast<float>(col) * div_side_len;

            // Stores the upper (with higher magnitude, not positionally above)
            // z coordinate of the current division.
            auto const hi_z = static_cast<float>(col + 1) * div_side_len;

            // First we generate the first half of the division.
            vertices.emplace_back(side_len, hi_y, hi_z);
            vertices.emplace_back(side_len, lo_y, hi_z);
            vertices.emplace_back(side_len, hi_y, lo_z);

            // Then we generate the second.
            vertices.emplace_back(side_len, hi_y, lo_z);
            vertices.emplace_back(side_len, lo_y, hi_z);
            vertices.emplace_back(side_len, lo_y, lo_z);
        }
    }

    // Generate the top plane.
    // We iterate division by division.
    // The outer loop traverses the z axis and the inner the loop traverses the
    // x axis.
    for (auto row = 0_u32; row < num_divs; ++row) {
        // Stores the lower (with lower magnitude, not positionally below) z
        // coordinate of the current division.
        auto const lo_z = static_cast<float>(row) * div_side_len;

        // Stores the upper (with higher magnitude, not positionally above) z
        // coordinate of the current division.
        auto const hi_z = static_cast<float>(row + 1) * div_side_len;

        for (auto col = 0_u32; col < num_divs; ++col) {
            // Stores the lower (with lower magnitude, not positionally below) x
            // coordinate of the current division.
            auto const lo_x = static_cast<float>(col) * div_side_len;

            // Stores the upper (with higher magnitude, not positionally above)
            // x coordinate of the current division.
            auto const hi_x = static_cast<float>(col + 1) * div_side_len;

            // First we generate the first half of the division.
            vertices.emplace_back(lo_x, side_len, lo_z);
            vertices.emplace_back(lo_x, side_len, hi_z);
            vertices.emplace_back(hi_x, side_len, lo_z);

            // Then we generate the second.
            vertices.emplace_back(hi_x, side_len, lo_z);
            vertices.emplace_back(lo_x, side_len, hi_z);
            vertices.emplace_back(hi_x, side_len, hi_z);
        }
    }

    // Generate the bottom plane.
    // We iterate division by division.
    // The outer loop traverses the z axis and the inner the loop traverses the
    // x axis.
    for (auto row = 0_u32; row < num_divs; ++row) {
        // Stores the lower (with lower magnitude, not positionally below) z
        // coordinate of the current division.
        auto const lo_z = static_cast<float>(row) * div_side_len;

        // Stores the upper (with higher magnitude, not positionally above) z
        // coordinate of the current division.
        auto const hi_z = static_cast<float>(row + 1) * div_side_len;

        for (auto col = 0_u32; col < num_divs; ++col) {
            // Stores the lower (with lower magnitude, not positionally below) x
            // coordinate of the current division.
            auto const lo_x = static_cast<float>(col) * div_side_len;

            // Stores the upper (with higher magnitude, not positionally above)
            // x coordinate of the current division.
            auto const hi_x = static_cast<float>(col + 1) * div_side_len;

            // Since this plane is contained in the xOz plane, every vertex's
            // y coordinate is zero.

            // First we generate the first half of the division.
            vertices.emplace_back(lo_x, 0.f, hi_z);
            vertices.emplace_back(lo_x, 0.f, lo_z);
            vertices.emplace_back(hi_x, 0.f, hi_z);

            // Then we generate the second.
            vertices.emplace_back(hi_x, 0.f, hi_z);
            vertices.emplace_back(lo_x, 0.f, lo_z);
            vertices.emplace_back(hi_x, 0.f, lo_z);
        }
    }

    // To ensure the box is centered at the origin, we need to perform a
    // translation of -side_len / 2.
    for (
        // Stores half of the box side length.
        auto const half_side_len = side_len / 2.f;
        auto&& vertex : vertices
    ) {
        vertex -= half_side_len;
    }

    return vertices;

} catch (std::bad_alloc const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
} catch (std::length_error const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
}

auto generate_and_print_box(
    float const side_len,
    u32 const num_divs,
    fmt::ostream& output_file
) noexcept -> cpp::result<void, GeneratorErr>
try {
    auto const& vertices = TRY_RESULT(generate_box(side_len, num_divs));
    output_file.print("{}\n", vertices.size());
    for (auto const& vertex : vertices) {
        output_file.print("{} {} {}\n", vertex.x, vertex.y, vertex.z);
    }
    return {};
} catch (...) {
    return cpp::fail(GeneratorErr::IO_ERR);
}

} // namespace generator
