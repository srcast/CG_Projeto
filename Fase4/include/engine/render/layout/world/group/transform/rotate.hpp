#pragma once

#include <glm/vec4.hpp>

namespace engine::render {

struct Rotate {
    enum class Kind {
        Angle,
        Time
    } kind;
    glm::vec4 rotate;
};

} // namespace engine::render
