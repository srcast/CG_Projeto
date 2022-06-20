#pragma once

#include "generator/err/err.hpp"

#include <brief_int.hpp>
#include <fmt/os.h>
#include <result.hpp>

namespace generator {

auto generate_plane(
    float side_len,
    brief_int::u32 num_divs,
    fmt::ostream& output_file
) noexcept -> cpp::result<void, generator_err>;

}

// namespace generator
