#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace engine::render {

struct transform {
    enum class kind {
        translate,
        rotate,
        scale
    } kind;

    union {
        glm::vec3 translate;
        glm::vec4 rotate;
        glm::vec3 scale;
    };
};

} // namespace engine::render
