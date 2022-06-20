#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/render/layout/world/group/model.hpp"

#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml {

auto parse_model(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<render::Model, ParseErr>;

} // namespace engine::parse::xml
