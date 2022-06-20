#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/render/world/camera.hpp"

#include <rapidxml.hpp>
#include <result.hpp>

namespace engine::parse::xml {

auto parse_camera(rapidxml::xml_node<> const* node) noexcept
    -> cpp::result<render::camera, parse_err>;

} // namespace engine::parse::xml
