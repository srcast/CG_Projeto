#pragma once

#include "generator/err/err.hpp"

#include <brief_int.hpp>
#include <fmt/os.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <result.hpp>
#include <vector>

namespace generator {

auto generate_sphere(
    float radius,
    brief_int::u32 num_slices,
    brief_int::u32 num_stacks
) noexcept -> cpp::result<std::vector<glm::vec3>, GeneratorErr>;

auto generate_and_print_sphere(
    float radius,
    brief_int::u32 num_slices,
    brief_int::u32 num_stacks,
    fmt::ostream& output_file
) noexcept -> cpp::result<void, GeneratorErr>;

} // namespace generator
