#pragma once

#include "engine/render/world/world.hpp"

#include <GL/freeglut.h>
#include <array>
#include <glm/ext/scalar_constants.hpp>
#include <glm/vec4.hpp>
#include <limits>
#include <string_view>

namespace engine {

namespace config {

auto inline constexpr PROG_NAME = std::string_view{"engine"};

} // namespace config

namespace render::config {

char inline constexpr WIN_TITLE[] = "engine";

enum : int {
    WIN_POS_X = 100,
    WIN_POS_Y = 100,
    WIN_WIDTH = 800,
    WIN_HEIGHT = 800,
};

static_assert(
    WIN_HEIGHT > 0,
    "WIN_HEIGHT must be greater than zero to prevent divide by zero when "
        "calculating aspect ratio."
);

auto constexpr ASPECT_RATIO
    = static_cast<double>(WIN_WIDTH)
    / static_cast<double>(WIN_WIDTH);

// Pleasant black.
auto inline constexpr DEFAULT_BG_COLOR = glm::vec4 {
    0.08f, // R
    0.08f, // G
    0.08f, // B
    1.f,   // A
};

// White.
auto inline constexpr DEFAULT_FG_COLOR = glm::vec4{1.f, 1.f, 1.f, 1.f};

auto inline constexpr ENABLE_AXIS = false;

auto inline constexpr DEFAULT_LINE_WIDTH = 1.f;
auto inline constexpr LINE_WIDTH_MIN = 1.f;
auto inline constexpr LINE_WIDTH_MAX = 5.f;
auto inline constexpr LINE_WIDTH_STEP = 1.f;

enum : GLenum { DEFAULT_POLYGON_MODE = GL_LINE };

enum kb_keys : unsigned char {
    KEY_ROTATE_UP = 'w',
    KEY_ROTATE_LEFT = 'a',
    KEY_ROTATE_DOWN = 's',
    KEY_ROTATE_RIGHT = 'd',
    KEY_ZOOM_IN = 'e',
    KEY_ZOOM_OUT = 'q',
    KEY_TOGGLE_AXIS = '.',
    KEY_NEXT_POLYGON_MODE = 'm',
    KEY_THINNER_LINES = '_',
    KEY_THICKER_LINES = '*',
};

auto inline constexpr Y_AXIS_HALF_LEN = 50.f;
auto inline constexpr Z_AXIS_HALF_LEN = 50.f;
auto inline constexpr X_AXIS_HALF_LEN = 50.f;

auto inline constexpr AXIS_COLOR = std::to_array<glm::vec3>({
    {1.f, 0.f, 0.f}, // x axis (red)
    {0.f, 1.f, 0.f}, // y axis (green)
    {0.f, 0.f, 1.f}, // z axis (blue)
});

auto inline const DEFAULT_WORLD = world {
    .camera = {
        .pos = {9., 3., 30.},
        .lookat = {0., 0., 0.},
        .up = {0., 1., 0.},
        .projection = {90., 0.5, 1000.}
    },
    .root = {},
};

enum : unsigned int { RENDER_TICK_MILLIS = 16 }; // 60 FPS

auto inline constexpr CAM_ROTATE_STEP = 0.01f;
auto inline constexpr CAM_VERT_ANGLE_MIN = 0.001f;
auto inline constexpr CAM_VERT_ANGLE_MAX = glm::pi<float>() - 0.001f;

auto inline constexpr CAM_ZOOM_STEP = 1.0f;
auto inline constexpr CAM_ZOOM_MIN
    = std::numeric_limits<float>::max() - 2.f * CAM_ZOOM_STEP;
auto inline constexpr CAM_ZOOM_MAX = 0.001f;

} // namespace render::config

} // namespace engine
