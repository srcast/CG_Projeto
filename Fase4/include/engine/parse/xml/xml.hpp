#pragma once

#include "engine/parse/xml/err/err.hpp"
#include "engine/render/layout/world/camera.hpp"
#include "engine/render/layout/world/world.hpp"

#include <result.hpp>
#include <utility>

namespace engine::parse::xml {

auto parse_xml(char const* xml_filepath) noexcept
    -> cpp::result<
        std::pair<render::World, render::Camera>,
        ParseErr
    >;

} // namespace engine::parse::xml
