#pragma once

#include "engine/render/layout/world/camera.hpp"
#include "engine/render/layout/world/world.hpp"

namespace engine::render {

class Renderer;

auto get() -> Renderer&;

class Renderer {
  private:
    auto friend get() -> Renderer&;

    Renderer();

  public:
    auto set_world(World& world) -> Renderer&;

    auto set_camera(Camera& camera) -> Renderer&;

    auto run() noexcept -> void;
};

} // namespace engine::render
