#include "engine/parse/xml/group/transform/rotate.hpp"

#include "engine/parse/xml/util/number_attr.hpp"
#include "util/try.hpp"

namespace engine::parse::xml {

auto parse_rotate(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<glm::vec4, parse_err>
{
    return glm::vec4 {
        TRY_RESULT(parse_number_attr<float>(node, "angle")),
        TRY_RESULT(parse_number_attr<float>(node, "x")),
        TRY_RESULT(parse_number_attr<float>(node, "y")),
        TRY_RESULT(parse_number_attr<float>(node, "z")),
    };
}

} // namespace engine::parse::xml
