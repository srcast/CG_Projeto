#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "util/number.hpp"
#include "util/parse_number.hpp"
#include "util/try.hpp"

#include <rapidxml.hpp>
#include <result.hpp>
#include <string_view>

namespace engine::parse::xml {

template <::util::number N>
auto constexpr parse_number_attr(
    rapidxml::xml_node<> const* const node,
    std::string_view const attr_name
) noexcept
    -> cpp::result<N, parse_err>
{
    if (auto const* const attr
            = node->first_attribute(attr_name.data(), attr_name.length());
        attr == nullptr
    ) {
        // if attribute isn't present, assume a value of zero.
        return N{};
    } else {
        return TRY_OPTION_OR(
            ::util::parse_number<N>(
                std::string_view {
                    attr->value(),
                    attr->value_size(),
                }
            ),
            return cpp::fail(parse_err::malformed_num)
        );
    }
}

} // engine::parse::xml
