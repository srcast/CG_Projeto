#pragma once

#include <brief_int.hpp>

namespace generator {

enum class GeneratorErr : brief_int::usize {
    NO_MEM,
    IO_ERR,

    MALFORMED_NUM,

    BOX_ZERO_DIVS,

    CONE_LT_THREE_SLICES,
    CONE_ZERO_STACKS,

    PLANE_ZERO_DIVS,

    SPHERE_LT_THREE_SLICES,
    SPHERE_LT_TWO_STACKS,

    BEZIER_LT_ONE_PATCH,
    BEZIER_LT_ONE_CTRL_POINT,
};

} // namespace generator
