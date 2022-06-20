#pragma once

#include <glm/vec3.hpp>
#include <vector>

namespace engine::render {

struct Model {
    std::vector<glm::vec3> vertices;
};

} // namespace engine::render
