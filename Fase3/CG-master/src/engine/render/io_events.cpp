#include "engine/render/io_events.hpp"

#include "engine/config.hpp"
#include "engine/render/state.hpp"

#include <algorithm>
#include <cctype>
#include <intrinsics/branching.hpp>

namespace engine::render {

auto key_down(unsigned char const key, int, int) noexcept -> void {
    state::keyboard.press(key);
    switch (key) {
        using enum config::KeyboardKeybinds;

        case KEY_TOGGLE_AXIS:
            state::enable_axis = not state::enable_axis;
            glutPostRedisplay();
            break;

        case KEY_TOGGLE_LOOKAT_INDICATOR:
            state::enable_lookat_indicator = not state::enable_lookat_indicator;
            glutPostRedisplay();
            break;

        case KEY_NEXT_POLYGON_MODE:
            using state::polygon_mode;
            switch (polygon_mode) {
                case GL_POINT:
                    polygon_mode = GL_LINE;
                    break;
                case GL_LINE:
                    polygon_mode = GL_FILL;
                    break;
                case GL_FILL:
                    polygon_mode = GL_POINT;
                    break;
                default:
                    intrinsics::unreachable();
            }
            glutPostRedisplay();
            break;

        case KEY_THINNER_LINES:
            state::line_width = std::max(
                state::line_width - config::LINE_WIDTH_STEP,
                config::LINE_WIDTH_MIN
            );
            glutPostRedisplay();
            break;

        case KEY_THICKER_LINES:
            state::line_width = std::min(
                state::line_width + config::LINE_WIDTH_STEP,
                config::LINE_WIDTH_MAX
            );
            glutPostRedisplay();
            break;

        case KEY_FOCUS_NEXT_MODEL:
            switch (state::camera_mode) {
                using enum CameraMode;

                case FREE_MODE:
                    state::camera_mode = FOLLOW_MODE;
                    break;

                case FOLLOW_MODE:
                    state::camera_mode = FREE_MODE;
                    break;
            }
            glutPostRedisplay();
        break;

        case KEY_EXIT_FOCUS_MODE:
            state::camera_mode = CameraMode::FREE_MODE;
            break;

        default:
            break;
    }
}

auto key_up(unsigned char key, int, int) noexcept -> void {
    if (std::isalpha(key)) {
        key = static_cast<unsigned char>(tolower(key));
    }
    state::keyboard.release(key);
}

} // namespace engine::render
