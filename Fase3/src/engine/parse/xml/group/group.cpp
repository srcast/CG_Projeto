#include "engine/parse/xml/group/group.hpp"

#include "engine/parse/xml/group/model/model_list.hpp"
#include "engine/parse/xml/group/transform/transform_list.hpp"
#include "util/try.hpp"

#include <new>
#include <stdexcept>
#include <string_view>

namespace engine::parse::xml {

// TODO: Implement non-recursively.
auto parse_group(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<render::Group, ParseErr>
try {
    using namespace std::string_view_literals;

    auto static constexpr transform_str = "transform"sv;
    auto static constexpr models_str = "models"sv;
    auto static constexpr group_str = "group"sv;

    auto root = render::Group{};

    for (
        auto const* child = node->first_node();
        child != nullptr;
        child = child->next_sibling()
    ) {
        auto const child_name = std::string_view {
            child->name(),
            child->name_size(),
        };

        if (child_name == transform_str) {
            root.transforms = TRY_RESULT(parse_transform_list(child));
        } else if (child_name == models_str) {
            root.models = TRY_RESULT(parse_model_list(child));
        } else if (child_name == group_str) {
            root.children.push_back(TRY_RESULT(parse_group(child)));
        } else {
            return cpp::fail(ParseErr::UNKNOWN_GROUP_CHILD_NODE);
        }
    }

    return root;

} catch (std::bad_alloc const&) {
    return cpp::fail(ParseErr::NO_MEM);
} catch (std::length_error const&) {
    return cpp::fail(ParseErr::NO_MEM);
}

} // namespace engine::parse::xml
