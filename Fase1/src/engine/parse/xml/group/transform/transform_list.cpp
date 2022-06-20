#include "engine/parse/xml/group/transform/transform_list.hpp"

#include "engine/parse/xml/group/transform/transform.hpp"
#include "util/try.hpp"

#include <new>
#include <stdexcept>

namespace engine::parse::xml {

[[nodiscard]]
auto parse_transform_list(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<std::vector<render::transform>, parse_err>
try {
    auto transform_list = std::vector<render::transform>{};

    for (
        auto const* transform = node->first_node();
        transform != nullptr;
        transform = transform->next_sibling()
    ) {
        transform_list.push_back(TRY_RESULT(parse_transform(node)));
    }

    return transform_list;

} catch (std::bad_alloc const&) {
    return cpp::failure{parse_err::no_mem};
} catch (std::length_error const&) {
    return cpp::failure{parse_err::no_mem};
}

} // engine::parse::xml
