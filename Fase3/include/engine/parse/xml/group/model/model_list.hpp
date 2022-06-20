#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/parse/xml/group/model/model.hpp"
#include "engine/render/layout/world/group/model.hpp"

#include <rapidxml.hpp>
#include <result.hpp>
#include <vector>

namespace engine::parse::xml {

auto parse_model_list(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<std::vector<render::Model>, ParseErr>;

} // namespace engine::parse::xml
