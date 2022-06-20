#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/render/world/group/model.hpp"

#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml {

auto parse_model(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<render::model, parse_err>;

} // namespace engine::parse::xml
