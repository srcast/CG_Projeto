#include "engine/parse/xml/group/transform/translate.hpp"

#include "engine/parse/xml/util/xyz.hpp"
#include "util/parse_number.hpp"
#include "util/try.hpp"

#include <brief_int.hpp>
#include <glm/vec3.hpp>
#include <new>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

namespace engine::parse::xml {

auto parse_translate(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<render::Translate, ParseErr>
try {
    using namespace brief_int;
    using namespace std::string_view_literals;

    auto static constexpr false_str = "False"sv;
    auto static constexpr true_str = "True"sv;
    auto static constexpr time_str = "time"sv;
    auto static constexpr align_str = "align"sv;

    auto const* const time_attr
        = node->first_attribute(time_str.data(), time_str.length());

    if (time_attr == nullptr) {
        // No time attribute, try to parse a static translate.
        return render::StaticTranslate{TRY_RESULT(util::parse_xyz(node))};
    }

    auto const time = TRY_OPTION_OR(
        ::util::parse_number<u32>(
            std::string_view {
                time_attr->value(),
                time_attr->value_size(),
            }
        ),
        return cpp::fail(ParseErr::MALFORMED_NUM)
    );

    auto const* const align_attr = TRY_NULLABLE_OR(
        node->first_attribute(align_str.data(), align_str.length()),
        return cpp::fail(ParseErr::NO_TRANSLATE_ALIGN_ATTR)
    );

    auto const align_attr_val = std::string_view {
        align_attr->value(),
        align_attr->value_size(),
    };

    bool align;
    if (align_attr_val == false_str) {
        align = false;
    } else if (align_attr_val == true_str) {
        align = true;
    } else {
        return cpp::fail(ParseErr::UNKNOWN_TRANSLATE_ALIGN_ATTR_VALUE);
    }

    auto points = std::vector<glm::vec3>{};
    points.reserve(4); // at least 4 points required.

    for (
        auto const* point = node->first_node();
        point != nullptr;
        point = point->next_sibling()
    ) {
        points.push_back(TRY_RESULT(util::parse_xyz(point)));
    }

    if (points.size() < 4) {
        // at least 4 points required.
        return cpp::fail(ParseErr::TRANSLATE_POINTS_LT_4);
    }

    return render::DynamicTranslate {
        .time = time,
        .align = align,
        .points = std::move(points),
    };

} catch (std::bad_alloc const&) {
    return cpp::fail(ParseErr::NO_MEM);
} catch (std::length_error const&) {
    return cpp::fail(ParseErr::NO_MEM);
}

} // namespace engine::parse::xml
