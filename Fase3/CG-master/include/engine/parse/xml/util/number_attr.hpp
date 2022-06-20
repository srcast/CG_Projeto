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
    -> cpp::result<N, ParseErr>
{
    if (auto const* const attr
            = node->first_attribute(attr_name.data(), attr_name.length());
        attr == nullptr
    ) {
        return cpp::fail(ParseErr::MALFORMED_NUM);
    } else {
        return TRY_OPTION_OR(
            ::util::parse_number<N>(
                std::string_view {
                    attr->value(),
                    attr->value_size(),
                }
            ),
            return cpp::fail(ParseErr::MALFORMED_NUM)
        );
    }
}

} // engine::parse::xml
