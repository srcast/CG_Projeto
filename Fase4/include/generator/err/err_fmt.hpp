#pragma once

#include "generator/err/err.hpp"

#include <fmt/format.h>
#include <intrinsics/branching.hpp>

template <>
struct fmt::formatter<generator::GeneratorErr> {
    using GeneratorErr = ::generator::GeneratorErr;

    auto constexpr parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto constexpr format(GeneratorErr const err, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", [err] {
            switch (err) {
                using enum ::generator::GeneratorErr;
                case NO_MEM:
                    return "ran out of memory";
                case IO_ERR:
                    return "input/output error";

                case MALFORMED_NUM:
                    return "malformed number";

                case BOX_ZERO_DIVS:
                    return "attempted to generate a box with zero divisions";

                case CONE_LT_THREE_SLICES:
                    return "attempted to generate a cone with less than three "
                        "slices";
                case CONE_ZERO_STACKS:
                    return "attempted to generate a cone with zero stacks";

                case PLANE_ZERO_DIVS:
                    return "attempted to generate a plane with zero divisions";

                case SPHERE_LT_THREE_SLICES:
                    return "attempted to generate a sphere with less than "
                        "three slices";
                case SPHERE_LT_TWO_STACKS:
                    return "attempted to generate a sphere with less than two "
                        "stacks";

                case BEZIER_LT_ONE_PATCH:
                    return "attempted to generate a bezier patch with less than"
                    " one patch";
                case BEZIER_LT_ONE_CTRL_POINT:
                    return "attempted to generate a bezier patch with less than"
                        "one control point";

                default:
                    intrinsics::unreachable();
            }
        }());
    }
};
