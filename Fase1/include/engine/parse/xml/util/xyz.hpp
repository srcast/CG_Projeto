#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/parse/xml/util/fp_attr.hpp"

#include <concepts>
#include <glm/vec3.hpp>
#include <rapidxml.hpp>
#include <result.hpp>
#include <type_traits>

namespace engine::parse::xml {

template <std::floating_point FP>
using xyz_ret
    = std::conditional_t<std::is_same_v<FP, float>, // if FP == float
        glm::vec3,                   // then return type is glm::vec3,
        glm::dvec3                   // else it's glm::dvec3.
    >;

template <std::floating_point FP>
[[nodiscard]]
auto constexpr parse_xyz(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<xyz_ret<FP>, parse_err>
{
    return xyz_ret<FP> {
        TRY_RESULT(parse_fp_attr<FP>(node, "x")),
        TRY_RESULT(parse_fp_attr<FP>(node, "y")),
        TRY_RESULT(parse_fp_attr<FP>(node, "z")),
    };
}

} // namespace engine::parse::xml
