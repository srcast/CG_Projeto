#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/render/layout/world/group/transform/rotate.hpp"

#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml {

auto parse_rotate(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<render::Rotate, ParseErr>;

} // namespace engine::parse::xml
