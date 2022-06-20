#pragma once

#include "engine/render/camera.hpp"
#include "engine/render/layout/world/camera.hpp"
#include "engine/render/layout/world/world.hpp"

#include <GL/freeglut.h>
#include <array>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string_view>

namespace engine {

namespace config {

extern constinit std::string_view const PROG_NAME;

} // namespace config

namespace render::config {

// WARNING: requires a terminating NULL byte.
extern constinit char const WIN_TITLE[];

extern constinit int const WIN_POS_X;
extern constinit int const WIN_POS_Y;
extern constinit int const WIN_WIDTH;
extern constinit int const WIN_HEIGHT;
extern constinit double const ASPECT_RATIO;

extern constinit glm::vec4 const DEFAULT_BG_COLOR;
extern constinit glm::vec4 const DEFAULT_FG_COLOR;

extern constinit bool const ENABLE_AXIS;
extern constinit float const Y_AXIS_HALF_LEN;
extern constinit float const Z_AXIS_HALF_LEN;
extern constinit float const X_AXIS_HALF_LEN;
extern constinit std::array<glm::vec3, 3> const AXIS_COLOR;

extern constinit bool const ENABLE_LOOKAT_INDICATOR;
extern constinit glm::vec3 const LOOKAT_INDICATOR_COLOR;

extern constinit float const DEFAULT_LINE_WIDTH;
extern constinit float const LINE_WIDTH_MIN;
extern constinit float const LINE_WIDTH_MAX;
extern constinit float const LINE_WIDTH_STEP;

extern constinit GLenum const DEFAULT_POLYGON_MODE;

enum KeyboardKeybinds : unsigned char {
    KEY_MOVE_FORWARD  = 'w',
    KEY_MOVE_LEFT     = 'a',
    KEY_MOVE_BACKWARD = 's',
    KEY_MOVE_RIGHT    = 'd',

    KEY_ROTATE_UP    = 'w',
    KEY_ROTATE_LEFT  = 'a',
    KEY_ROTATE_DOWN  = 's',
    KEY_ROTATE_RIGHT = 'd',

    KEY_ZOOM_IN = 'e',
    KEY_ZOOM_OUT = 'q',

    KEY_TOGGLE_AXIS = 'x',
    KEY_TOGGLE_LOOKAT_INDICATOR = '.',
    KEY_NEXT_POLYGON_MODE = 'm',
    KEY_THINNER_LINES = '-',
    KEY_THICKER_LINES = '+',
    KEY_FOCUS_NEXT_MODEL = '\t',
    KEY_EXIT_FOCUS_MODE = 27, // ESC key.
};

extern constinit unsigned int const RENDER_TICK_MILLIS;

// WARNING: not constinit, do not rely on initialization order!
extern World const DEFAULT_WORLD;

extern constinit Camera const DEFAULT_CAMERA;

extern constinit CameraMode const DEFAULT_CAMERA_MODE;

extern constinit float const CAMERA_ROTATE_STEP;
extern constinit float const CAMERA_VERT_ANGLE_MIN;
extern constinit float const CAMERA_VERT_ANGLE_MAX;

extern constinit float const CAMERA_ZOOM_STEP;
extern constinit float const CAMERA_ZOOM_MIN;
extern constinit float const CAMERA_ZOOM_MAX;

} // namespace render::config

} // namespace engine
