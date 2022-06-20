#pragma once

#include "engine/render/world/camera.hpp"
#include "engine/render/world/group/group.hpp"

namespace engine::render {

struct world {
    struct camera camera;
    group root;
};

} // namespace engine::render
