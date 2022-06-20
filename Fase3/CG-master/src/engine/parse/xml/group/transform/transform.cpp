#include "engine/parse/xml/group/transform/transform.hpp"

#include "engine/parse/xml/group/transform/rotate.hpp"
#include "engine/parse/xml/group/transform/translate.hpp"
#include "engine/parse/xml/util/xyz.hpp"
#include "util/try.hpp"

#include <string_view>

namespace engine::parse::xml {

auto parse_transform(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<render::Transform, ParseErr>
{
    using namespace std::string_view_literals;

    auto static constexpr translate_str = "translate"sv;
    auto static constexpr rotate_str = "rotate"sv;
    auto static constexpr scale_str = "scale"sv;

    auto const transform_name = std::string_view {
        node->name(),
        node->name_size(),
    };

    if (transform_name == translate_str) {
        return render::Translate{TRY_RESULT(parse_translate(node))};
    } else if (transform_name == rotate_str) {
        return render::Rotate{TRY_RESULT(parse_rotate(node))};
    } else if (transform_name == scale_str) {
        return render::Scale{TRY_RESULT(util::parse_xyz(node))};
    } else {
        return cpp::fail(ParseErr::UNKNOWN_TRANSFORM);
    }
}

} // namespace engine::parse::xml
