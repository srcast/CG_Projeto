#pragma once

#include "engine/parse/xml/err/err.hpp"

#include <glm/vec4.hpp>
#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml {

auto parse_rotate(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<glm::vec4, parse_err>;

} // namespace engine::parse::xml
