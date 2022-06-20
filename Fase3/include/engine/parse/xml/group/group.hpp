#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/render/layout/world/group/group.hpp"

#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml {

auto parse_group(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<render::Group, ParseErr>;

} // namespace engine::parse::xml
