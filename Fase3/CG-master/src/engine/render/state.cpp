#include "engine/render/state.hpp"

#include "engine/config.hpp"
#include "engine/render/layout/world/group/group.hpp"

namespace engine::render::state {

bool enable_axis = config::ENABLE_AXIS;
bool enable_lookat_indicator = config::ENABLE_LOOKAT_INDICATOR;

GLenum polygon_mode = config::DEFAULT_POLYGON_MODE;
float line_width = config::DEFAULT_LINE_WIDTH;

Keyboard keyboard = {};

World default_world_mut = config::DEFAULT_WORLD;
ptr::nonnull_ptr<World> world_ptr = ptr::nonnull_ptr_to(default_world_mut);

Camera default_camera_mut = config::DEFAULT_CAMERA;
ptr::nonnull_ptr<Camera> camera_ptr = ptr::nonnull_ptr_to(default_camera_mut);
enum CameraMode camera_mode;

std::vector<std::vector<float>> buffers;

GLuint bind[500];

} // namespace engine::render::state
