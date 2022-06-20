#pragma once

#include "engine/parse/xml/err/err.hpp"

#include <glm/vec3.hpp>
#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml {

[[nodiscard]]
auto parse_projection(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<glm::dvec3, parse_err>;

} // namespace engine::parse::xml
