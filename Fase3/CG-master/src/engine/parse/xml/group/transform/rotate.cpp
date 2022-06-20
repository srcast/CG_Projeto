#include "engine/parse/xml/group/transform/rotate.hpp"

#include "engine/parse/xml/util/number_attr.hpp"
#include "util/try.hpp"

namespace engine::parse::xml {

auto parse_rotate(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<render::Rotate, ParseErr>
{
    using Kind = render::Rotate::Kind;

    Kind kind;
    auto fst = parse_number_attr<float>(node, "angle");
    if (fst.has_value()) {
        kind = Kind::Angle;
    } else {
        fst = parse_number_attr<float>(node, "time");
        kind = Kind::Time;
    }

    return render::Rotate {
        .kind = kind,
        .rotate = {
            TRY_RESULT(fst),
            TRY_RESULT(parse_number_attr<float>(node, "x")),
            TRY_RESULT(parse_number_attr<float>(node, "y")),
            TRY_RESULT(parse_number_attr<float>(node, "z")),
        }
    };
}

} // namespace engine::parse::xml
