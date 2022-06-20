#include "engine/parse/xml/util/xyz.hpp"

#include "engine/parse/xml/util/number_attr.hpp"
#include "util/try.hpp"

namespace engine::parse::xml::util {

auto parse_xyz(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<glm::vec3, parse_err>
{
    return glm::vec3 {
        TRY_RESULT(parse_number_attr<float>(node, "x")),
        TRY_RESULT(parse_number_attr<float>(node, "y")),
        TRY_RESULT(parse_number_attr<float>(node, "z")),
    };
}

} // namespace engine::parse::xml::util
