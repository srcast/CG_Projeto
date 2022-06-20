#include "engine/render/camera.hpp"

#include "engine/config.hpp"
#include "engine/render/state.hpp"
#include "util/coord_conv.hpp"

#include <algorithm>
#include <glm/geometric.hpp>

namespace engine::render {

auto static update_camera_free_mode() noexcept -> void;
auto static update_camera_follow_mode() noexcept -> void;

auto update_camera() noexcept -> void {
    switch (state::camera_mode) {
        using enum CameraMode;
        case FREE_MODE:
            update_camera_free_mode();
            break;
        case FOLLOW_MODE:
            update_camera_follow_mode();
            break;
    }
    glutPostRedisplay();
    glutTimerFunc(config::RENDER_TICK_MILLIS, [](int) { update_camera(); }, 0);

}

auto static update_camera_free_mode() noexcept -> void {
    using enum config::KeyboardKeybinds;

    auto const& keyboard = state::keyboard;
    auto& camera = *state::camera_ptr;

    auto const direction = glm::normalize(camera.lookat - camera.pos);
    auto const right = glm::normalize(glm::cross(direction, camera.up));

#   define FLT(x) static_cast<float>(x)
    auto const translation
        = direction * FLT(keyboard.pressed(KEY_MOVE_FORWARD))
        - direction * FLT(keyboard.pressed(KEY_MOVE_BACKWARD))
        + right * FLT(keyboard.pressed(KEY_MOVE_RIGHT))
        - right * FLT(keyboard.pressed(KEY_MOVE_LEFT));
#   undef FLT

    camera.pos += translation;
    camera.lookat += translation;
}

auto static update_camera_follow_mode() noexcept -> void {
    using enum config::KeyboardKeybinds;

    auto const& keyboard = state::keyboard;
    auto& camera = *state::camera_ptr;
    auto relative_pos = camera.pos - camera.lookat;

    ::util::cartesian_to_spherical_inplace(relative_pos);

    // Rotate left/right.
    if (keyboard.pressed(KEY_ROTATE_LEFT) and not keyboard.pressed(KEY_ROTATE_RIGHT)) {
        relative_pos[2] -= config::CAMERA_ROTATE_STEP;
    } else if (keyboard.pressed(KEY_ROTATE_RIGHT) and not keyboard.pressed(KEY_ROTATE_LEFT)) {
        relative_pos[2] += config::CAMERA_ROTATE_STEP;
    }

    // Rotate up/down.
    if (keyboard.pressed(KEY_ROTATE_UP) and not keyboard.pressed(KEY_ROTATE_DOWN)) {
        relative_pos[1] = std::max(
            config::CAMERA_VERT_ANGLE_MIN,
            relative_pos[1] - config::CAMERA_ROTATE_STEP
        );
    } else if (keyboard.pressed(KEY_ROTATE_DOWN) and not keyboard.pressed(KEY_ROTATE_UP)) {
        relative_pos[1] = std::min(
            config::CAMERA_VERT_ANGLE_MAX,
            relative_pos[1] + config::CAMERA_ROTATE_STEP
        );
    }

    // Zoom in/out.
    if (keyboard.pressed(KEY_ZOOM_IN) and not keyboard.pressed(KEY_ZOOM_OUT)) {
        relative_pos[0] = std::max(
            config::CAMERA_ZOOM_MAX,
            relative_pos[0] - config::CAMERA_ZOOM_STEP
        );
    } else if (keyboard.pressed(KEY_ZOOM_OUT) and not keyboard.pressed(KEY_ZOOM_IN)) {
        relative_pos[0] = std::min(
            config::CAMERA_ZOOM_MIN,
            relative_pos[0] + config::CAMERA_ZOOM_STEP
        );
    }

    ::util::spherical_to_cartesian_inplace(relative_pos);

    camera.pos = relative_pos + camera.lookat;
}

} // namespace engine::render
