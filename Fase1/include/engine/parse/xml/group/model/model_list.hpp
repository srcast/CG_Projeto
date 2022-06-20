#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/render/world/group/model.hpp"

#include <rapidxml.hpp>
#include <result.hpp>
#include <vector>

namespace engine::parse::xml {

[[nodiscard]]
auto parse_model_list(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<std::vector<render::model>, parse_err>;

} // namespace engine::parse::xml
