#include "engine/parse/xml/group/model/model_list.hpp"

#include "engine/render/layout/world/group/model.hpp"
#include "util/try.hpp"

#include <new>
#include <stdexcept>

namespace engine::parse::xml {

auto parse_model_list(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<std::vector<render::Model>, ParseErr>
try {
    auto model_list = std::vector<render::Model>{};

    for (
        auto const* model = node->first_node();
        model != nullptr;
        model = model->next_sibling()
    ) {
        model_list.push_back(TRY_RESULT(parse_model(model)));
    }

    return model_list;

} catch (std::bad_alloc const&) {
    return cpp::fail(ParseErr::NO_MEM);
} catch (std::length_error const&) {
    return cpp::fail(ParseErr::NO_MEM);
}

} // namespace engine::parse::xml
