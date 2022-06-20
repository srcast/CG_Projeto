#pragma once

#include <glm/vec3.hpp>

namespace engine::render {

struct camera {
    glm::dvec3 pos;
    glm::dvec3 lookat;
    glm::dvec3 up;
    glm::dvec3 projection;
};

} // namespace engine::render
