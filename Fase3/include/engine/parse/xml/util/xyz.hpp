#pragma once

#include "engine/parse/xml/err/err.hpp"

#include <glm/vec3.hpp>
#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml::util {

auto parse_xyz(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<glm::vec3, ParseErr>;

} // namespace engine::parse::xml::util
