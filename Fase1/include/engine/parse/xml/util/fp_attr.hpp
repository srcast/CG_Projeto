#pragma once

#include "engine/parse/util/fp.hpp"
#include "engine/parse/xml/err/err.hpp"
#include "util/try.hpp"

#include <concepts>
#include <rapidxml.hpp>
#include <result.hpp>
#include <string_view>

namespace engine::parse::xml {

template <std::floating_point FP>
[[nodiscard]]
auto constexpr parse_fp_attr(
    rapidxml::xml_node<> const* const node,
    std::string_view const attr_name
) noexcept
    -> cpp::result<FP, parse_err>
{
    if (auto const* const attr
            = node->first_attribute(attr_name.data(), attr_name.length());
        attr == nullptr
    ) {
        // if attribute isn't present, assume a value of zero.
        return FP{};
    } else {
        return TRY_OPTION_OR(
            util::parse_fp<FP>(
                std::string_view {
                    attr->value(),
                    attr->value_size(),
                }
            ),
            return cpp::failure{parse_err::malformed_num}
        );
    }
}

} // engine::parse::xml
