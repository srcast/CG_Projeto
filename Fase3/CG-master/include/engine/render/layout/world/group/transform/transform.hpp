#pragma once

#include "engine/render/layout/world/group/transform/rotate.hpp"
#include "engine/render/layout/world/group/transform/scale.hpp"
#include "engine/render/layout/world/group/transform/translate.hpp"

#include <variant>

namespace engine::render {

using Transform = std::variant<Translate, Rotate, Scale>;

} // namespace engine::render
