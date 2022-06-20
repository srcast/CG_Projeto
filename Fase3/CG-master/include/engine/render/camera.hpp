#pragma once

namespace engine::render {

enum class CameraMode {
    FREE_MODE,
    FOLLOW_MODE,
};

auto update_camera() noexcept -> void;

} // namespace engine::render
