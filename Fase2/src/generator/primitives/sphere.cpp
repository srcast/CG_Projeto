#include "generator/primitives/sphere.hpp"

#include <glm/ext/scalar_constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <new>
#include <stdexcept>
#include <vector>

using namespace brief_int;

namespace generator {

auto generate_sphere(
    float const radius,
    u32 const num_slices,
    u32 const num_stacks,
    fmt::ostream& output_file
) noexcept -> cpp::result<void, generator_err>
try {
    using namespace brief_int::literals;

    if (num_slices <= 2_u32) {
        // A sphere needs at least 3 slices to be properly generated.
        return cpp::fail(generator_err::sphere_lt_three_slices);
    }

    if (num_stacks <= 1_u32) {
        // A sphere needs at least 2 stacks to be properly generated.
        return cpp::fail(generator_err::sphere_lt_two_stacks);
    }

    auto const slice_angle
        = glm::pi<float>() * (2.f / static_cast<float>(num_slices));

    auto const stack_angle
        = glm::pi<float>() / static_cast<float>(num_stacks);

    auto const total_vertex_count
        = 6_uz
        * static_cast<usize>(num_stacks - 1_u32)
        * static_cast<usize>(num_slices);

    auto vertices = std::vector<glm::vec3>{};
    vertices.reserve(total_vertex_count);

    for (auto i = 0_u32; i < num_stacks; ++i) {
        auto static constexpr half_pi = glm::pi<float>() / 2.f;

        auto const curr_stack_angle
            = half_pi - static_cast<float>(i) * stack_angle;

        auto const next_stack_angle
            = half_pi - static_cast<float>(i + 1_u32) * stack_angle;

        for (auto j = 0_u32; j < num_slices; ++j) {
            auto const curr_slice_angle
                = static_cast<float>(j) * slice_angle;

            auto const next_slice_angle
                = static_cast<float>(j + 1_u32) * slice_angle;

            using glm::cos;
            using glm::sin;

            if (i < num_stacks - 1_u32) {
                vertices.emplace_back(
                    radius * cos(curr_stack_angle) * sin(curr_slice_angle),
                    radius * sin(curr_stack_angle),
                    radius * cos(curr_stack_angle) * cos(curr_slice_angle)
                );
                vertices.emplace_back(
                    radius * cos(next_stack_angle) * sin(curr_slice_angle),
                    radius * sin(next_stack_angle),
                    radius * cos(next_stack_angle) * cos(curr_slice_angle)
                );
                vertices.emplace_back(
                    radius * cos(next_stack_angle) * sin(next_slice_angle),
                    radius * sin(next_stack_angle),
                    radius * cos(next_stack_angle) * cos(next_slice_angle)
                );
            } else {
                vertices.emplace_back(
                    radius * cos(next_stack_angle) * sin(next_slice_angle),
                    radius * sin(next_stack_angle),
                    radius * cos(next_stack_angle) * cos(next_slice_angle)
                );
                vertices.emplace_back(
                    radius * cos(curr_stack_angle) * sin(next_slice_angle),
                    radius * sin(curr_stack_angle),
                    radius * cos(curr_stack_angle) * cos(next_slice_angle)
                );
                vertices.emplace_back(
                    radius * cos(curr_stack_angle) * sin(curr_slice_angle),
                    radius * sin(curr_stack_angle),
                    radius * cos(curr_stack_angle) * cos(curr_slice_angle)
                );
            }

            if (i > 0_u32 && i < num_stacks - 1_u32) {
                vertices.emplace_back(
                    radius * cos(next_stack_angle) * sin(next_slice_angle),
                    radius * sin(next_stack_angle),
                    radius * cos(next_stack_angle) * cos(next_slice_angle)
                );
                vertices.emplace_back(
                    radius * cos(curr_stack_angle) * sin(next_slice_angle),
                    radius * sin(curr_stack_angle),
                    radius * cos(curr_stack_angle) * cos(next_slice_angle)
                );
                vertices.emplace_back(
                    radius * cos(curr_stack_angle) * sin(curr_slice_angle),
                    radius * sin(curr_stack_angle),
                    radius * cos(curr_stack_angle) * cos(curr_slice_angle)
                );
            }
        }
    }

    output_file.print("{}\n",total_vertex_count);
    for (auto&& vertex : vertices) {
        output_file.print("{} {} {}\n", vertex.x, vertex.y, vertex.z);
    }

    return {};

} catch (std::bad_alloc const&) {
    return cpp::fail(generator_err::no_mem);
} catch (std::length_error const&) {
    return cpp::fail(generator_err::no_mem);
}

} // namespace generator
