#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/render/layout/world/group/transform/translate.hpp"

#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml {

auto parse_translate(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<render::Translate, ParseErr>;

} // namespace engine::parse::xml
