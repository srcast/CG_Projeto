#include "engine/parse/xml/group/transform/transform.hpp"

#include "engine/parse/xml/group/transform/rotate.hpp"
#include "engine/parse/xml/util/xyz.hpp"
#include "util/try.hpp"

#include <string_view>

namespace engine::parse::xml {

[[nodiscard]]
auto parse_transform(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<render::transform, parse_err>
{
    using namespace std::string_view_literals;
    using enum decltype(render::transform::kind); // disambiguate.

    auto static constexpr translate_str = "translate"sv;
    auto static constexpr rotate_str = "rotate"sv;
    auto static constexpr scale_str = "scale"sv;

    auto const transform_name = std::string_view {
        node->name(),
        node->name_size(),
    };

    if (transform_name == translate_str) {
        return render::transform {
            .kind = translate,
            .translate = TRY_RESULT(parse_xyz<float>(node)),
        };
    } else if (transform_name == rotate_str) {
        return render::transform {
            .kind = rotate,
            .rotate = TRY_RESULT(parse_rotate(node)),
        };
    } else if (transform_name == scale_str) {
        return render::transform {
            .kind = scale,
            .scale = TRY_RESULT(parse_xyz<float>(node)),
        };
    } else {
        return cpp::failure{parse_err::unknown_transform};
    }
}

} // namespace engine::parse::xml
