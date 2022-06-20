#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/render/world/group/transform.hpp"

#include <rapidxml.hpp>
#include <result.hpp>
#include <vector>

namespace engine::parse::xml {

[[nodiscard]]
auto parse_transform_list(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<std::vector<render::transform>, parse_err>;

} // engine::parse::xml
