#pragma once

#include "engine/parse/xml/err/err.hpp"

#include <array>
#include <fmt/format.h>
#include <string_view>
#include <type_traits>

template <>
struct fmt::formatter<engine::parse::xml::parse_err> {
    using parse_err = ::engine::parse::xml::parse_err;

    auto constexpr parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto constexpr format(parse_err const err, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", [err] {
            using namespace brief_int::literals;

            auto static constexpr to_string = std::to_array<std::string_view>({
                "ran out of memory",
                "input/output error",

                "XML syntax error",
                "malformed number",

                "no world node found",
                "no camera node found",
                "no root group node found",

                "no camera position node found",
                "no camera lookat node found",
                "no camera up node found",
                "no camera projection node found",

                "unrecognized group child node"

                "unrecognized transformation",

                "no model filename attribute",
                "model points to nonexistent file",
            });
            auto const idx
                = static_cast<std::underlying_type_t<parse_err>>(err);
            if (idx < 0_uz || idx > 14_uz) {
                __builtin_unreachable();
            }
            return to_string[idx];
        }());
    }
};
