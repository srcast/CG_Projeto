#include "generator/primitives/sphere.hpp"

#include "util/try.hpp"

#include <glm/ext/scalar_constants.hpp>
#include <glm/trigonometric.hpp>
#include <new>
#include <stdexcept>

namespace generator {

using namespace brief_int;

auto generate_sphere(
    float const radius,
    u32 const num_slices,
    u32 const num_stacks
) noexcept -> cpp::result<std::vector<glm::vec3>, GeneratorErr>
try {
    using namespace brief_int::literals;

    if (num_slices <= 2) {
        // A sphere needs at least 3 slices to be properly generated.
        return cpp::fail(GeneratorErr::SPHERE_LT_THREE_SLICES);
    }

    if (num_stacks <= 1) {
        // A sphere needs at least 2 stacks to be properly generated.
        return cpp::fail(GeneratorErr::SPHERE_LT_TWO_STACKS);
    }

    auto const slice_angle
        = glm::pi<float>() * (2.f / static_cast<float>(num_slices));

    auto const stack_angle
        = glm::pi<float>() / static_cast<float>(num_stacks);

    auto const total_vertex_count
        = 6
        * static_cast<usize>(num_stacks - 1)
        * static_cast<usize>(num_slices);

    auto vertices = std::vector<glm::vec3>{};
    vertices.reserve(total_vertex_count);

    for (auto i = 0_u32; i < num_stacks; ++i) {
        auto static constexpr half_pi = glm::pi<float>() / 2.f;

        auto const curr_stack_angle
            = half_pi - static_cast<float>(i) * stack_angle;

        auto const next_stack_angle
            = half_pi - static_cast<float>(i + 1) * stack_angle;

        for (auto j = 0_u32; j < num_slices; ++j) {
            auto const curr_slice_angle
                = static_cast<float>(j) * slice_angle;

            auto const next_slice_angle
                = static_cast<float>(j + 1) * slice_angle;

            using glm::cos;
            using glm::sin;

            if (i < num_stacks - 1) {
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

            if (i > 0 && i < num_stacks - 1) {
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

    return vertices;

} catch (std::bad_alloc const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
} catch (std::length_error const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
}

auto generate_and_print_sphere(
    float const radius,
    u32 const num_slices,
    u32 const num_stacks,
    fmt::ostream& output_file
) noexcept -> cpp::result<void, GeneratorErr>
try {
    auto const& vertices = TRY_RESULT(
        generate_sphere(radius, num_slices, num_stacks)
    );
    output_file.print("{}\n", vertices.size());
    for (auto const& vertex : vertices) {
        output_file.print("{} {} {}\n", vertex.x, vertex.y, vertex.z);
    }
    return {};
} catch (...) {
    return cpp::fail(GeneratorErr::IO_ERR);
}

} // namespace generator
