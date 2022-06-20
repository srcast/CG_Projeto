#pragma once

#include <charconv>
#include <concepts>
#include <optional>
#include <string_view>

namespace engine::parse::util {

template <std::floating_point FP>
[[nodiscard]]
auto constexpr parse_fp(std::string_view const s) noexcept
    -> std::optional<FP>
{
    FP uninit;
    char const* const begin = s.data();
    char const* const end = begin + s.length();
    auto const [parse_end, err] = std::from_chars(begin, end, uninit);
    if (parse_end != end || err != std::errc{}) {
        return {};
    }
    return uninit;
}

} // namespace engine::util
