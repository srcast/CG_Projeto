#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/render/layout/world/group/transform/transform.hpp"

#include <rapidxml.hpp>
#include <result.hpp>
#include <vector>

namespace engine::parse::xml {

auto parse_transform_list(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<std::vector<render::Transform>, ParseErr>;

} // engine::parse::xml
