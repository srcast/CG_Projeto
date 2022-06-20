#pragma once

#include <glm/vec3.hpp>

namespace engine::render {

struct camera {
    glm::vec3 pos;
    glm::vec3 lookat;
    glm::vec3 up;
    glm::vec3 projection;
};

} // namespace engine::render
