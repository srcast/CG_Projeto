#pragma once

#include "util/number.hpp"

#include <charconv>
#include <optional>
#include <string_view>

namespace util {

template <number N>
[[nodiscard]]
auto constexpr parse_number(std::string_view const s) noexcept
    -> std::optional<N>
{
    N uninit;
    char const* const begin = s.data();
    char const* const end = begin + s.length();
    auto const [parse_end, err] = std::from_chars(begin, end, uninit);
    if (parse_end != end || err != std::errc{}) {
        return {};
    }
    return uninit;
}

} // namespace util
