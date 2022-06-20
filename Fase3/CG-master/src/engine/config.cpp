#include "engine/config.hpp"

#include <glm/ext/scalar_constants.hpp>
#include <limits>

namespace engine {

namespace config {

constinit std::string_view const PROG_NAME = "engine";

} // namespace config

namespace render::config {

constinit char const WIN_TITLE[] = "engine";

constinit int const WIN_POS_X = 100;
constinit int const WIN_POS_Y = 100;
constinit int const WIN_WIDTH = 800;
constinit int const WIN_HEIGHT = 800;

static_assert(
    WIN_HEIGHT > 0,
    "WIN_HEIGHT must be greater than zero to prevent divide by zero when "
        "calculating aspect ratio."
);

constinit double const ASPECT_RATIO
    = static_cast<double>(WIN_WIDTH)
    / static_cast<double>(WIN_HEIGHT);

// Pleasant black.
constinit glm::vec4 const DEFAULT_BG_COLOR = {0.08f, 0.08f, 0.08f, 1.f};

// White.
constinit glm::vec4 const DEFAULT_FG_COLOR = {1.f, 1.f, 1.f, 1.f};

constinit bool const ENABLE_AXIS = false;
constinit float const Y_AXIS_HALF_LEN = 50.f;
constinit float const Z_AXIS_HALF_LEN = 50.f;
constinit float const X_AXIS_HALF_LEN = 50.f;

constinit std::array<glm::vec3, 3> const AXIS_COLOR = {{
    {1.f, 0.f, 0.f}, // x axis (red)
    {0.f, 1.f, 0.f}, // y axis (green)
    {0.f, 0.f, 1.f}, // z axis (blue)
}};

constinit bool const ENABLE_LOOKAT_INDICATOR = false;

// Yellow.
constinit glm::vec3 const LOOKAT_INDICATOR_COLOR = {1.f, 1.f, 0.f};

constinit float const DEFAULT_LINE_WIDTH = 1.f;
constinit float const LINE_WIDTH_MIN = 1.f;
constinit float const LINE_WIDTH_MAX = 5.f;
constinit float const LINE_WIDTH_STEP = 1.f;

constinit GLenum const DEFAULT_POLYGON_MODE = GL_LINE;

constinit unsigned int const RENDER_TICK_MILLIS = 16; // 60 FPS

// WARNING: not constinit, do not rely on initialization order!
World const DEFAULT_WORLD = {};

constinit Camera const DEFAULT_CAMERA = {
    .pos =        {10.f, 10.f, 10.f},
    .lookat =     {0.f,  0.f,  0.f},
    .up =         {0.f,  1.f,  0.f},
    .projection = {90.f, 0.5f, 1000.f},
};

constinit CameraMode const DEFAULT_CAMERA_MODE = CameraMode::FOLLOW_MODE;

constinit float const CAMERA_ROTATE_STEP = 0.01f;
constinit float const CAMERA_VERT_ANGLE_MIN = 0.001f;
constinit float const CAMERA_VERT_ANGLE_MAX = glm::pi<float>() - 0.001f;

constinit float const CAMERA_ZOOM_STEP = 1.0f;
constinit float const CAMERA_ZOOM_MIN
    = std::numeric_limits<float>::max() - 2.f * CAMERA_ZOOM_STEP;
constinit float const CAMERA_ZOOM_MAX = 0.001f;

} // namespace render::config

} // namespace engine
