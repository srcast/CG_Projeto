#pragma once

#include "engine/render/camera.hpp"
#include "engine/render/keyboard.hpp"
#include "engine/render/layout/world/camera.hpp"
#include "engine/render/layout/world/world.hpp"

#include <GL/freeglut.h>
#include <nonnull_ptr.hpp>
#include <vector>

namespace engine::render::state {

extern bool enable_axis;
extern bool enable_lookat_indicator;

extern GLenum polygon_mode;

extern float line_width;

extern Keyboard keyboard;

extern World default_world_mut;
extern ptr::nonnull_ptr<World> world_ptr;

extern Camera default_camera_mut;
extern ptr::nonnull_ptr<Camera> camera_ptr;
extern CameraMode camera_mode;

extern std::vector<std::vector<float>> buffers;
extern GLuint bind[3][500];

} // namespace engine::render::state
