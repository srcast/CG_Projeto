#pragma once

#include "engine/render/layout/world/group/model.hpp"
#include "engine/render/layout/world/group/transform/transform.hpp"

#include <vector>

namespace engine::render {

struct Group {
    std::vector<Transform> transforms;
    std::vector<Model> models;
    std::vector<Group> children;
};

} // namespace engine::render
