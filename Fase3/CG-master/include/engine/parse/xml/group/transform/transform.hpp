#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/render/layout/world/group/transform/transform.hpp"

#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml {

auto parse_transform(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<render::Transform, ParseErr>;

} // namespace engine::parse::xml
