#pragma once

#include "generator/err/err.hpp"

#include <fmt/format.h>

template <>
struct fmt::formatter<generator::generator_err> {
    using generator_err = ::generator::generator_err;

    auto constexpr parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto constexpr format(generator_err const err, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", [err] {
            switch (err) {
                using enum ::generator::generator_err;
                case no_mem:
                    return "ran out of memory";
                case io_err:
                    return "input/output error";

                case box_zero_divs:
                    return "attempted to generate a box with zero divisions";

                case cone_lt_three_slices:
                    return "attempted to generate a cone with less than three "
                        "slices";
                case cone_zero_stacks:
                    return "attempted to generate a cone with zero stacks";

                case plane_zero_divs:
                    return "attempted to generate a plane with zero divisions";

                case sphere_lt_three_slices:
                    return "attempted to generate a sphere with less than "
                        "three slices";
                case sphere_lt_two_stacks:
                    return "attempted to generate a sphere with less than two "
                        "stacks";
                default:
                    __builtin_unreachable();
            }
        }());
    }
};
