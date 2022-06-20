#pragma once

#include "generator/err/err.hpp"

#include <brief_int.hpp>
#include <fmt/os.h>
#include <result.hpp>

namespace generator {

auto generate_cone(
    float radius,
    float height,
    brief_int::u32 num_slices,
    brief_int::u32 num_stacks,
    fmt::ostream& output_file
) noexcept -> cpp::result<void, generator_err>;

} // namespace generator
