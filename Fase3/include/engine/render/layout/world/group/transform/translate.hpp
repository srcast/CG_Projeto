#pragma once

#include <brief_int.hpp>
#include <glm/vec3.hpp>
#include <variant>
#include <vector>

namespace engine::render {

struct StaticTranslate {
    glm::vec3 xyz;
};

struct DynamicTranslate {
    brief_int::u32 time;
    bool align;
    std::vector<glm::vec3> points; // at least 4 points required.
};

using Translate = std::variant<StaticTranslate, DynamicTranslate>;

} // namespace engine::render
