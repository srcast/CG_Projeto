#pragma once

#include <brief_int.hpp>
#include <fmt/os.h>

auto generate_cone(
    float radius,
    float height,
    brief_int::u32 num_slices,
    brief_int::u32 num_stacks,
    fmt::ostream& output_file
) -> void;
