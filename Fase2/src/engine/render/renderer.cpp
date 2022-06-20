#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "engine/render/renderer.hpp"

#include "engine/config.hpp"
#include "engine/render/keyboard.hpp"
#include "engine/render/world/world.hpp"
#include "util/coord_conv.hpp"

#include <GL/freeglut.h>
#include <algorithm>
#include <cctype>
#include <fmt/core.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <nonnull_ptr.hpp>
#include <spdlog/spdlog.h>

namespace engine::render {

auto render() noexcept -> void;
auto update_camera(int) noexcept -> void;
auto render_axis() noexcept -> void;
auto render_group(group const& root) noexcept -> void;
auto resize(int width, int height) noexcept -> void;
auto key_down(unsigned char key, int x, int y) noexcept -> void;
auto key_up(unsigned char key, int x, int y) noexcept -> void;
auto display_info() -> void;

namespace state {

auto static enable_axis = config::ENABLE_AXIS;
auto static polygon_mode = static_cast<GLenum>(config::DEFAULT_POLYGON_MODE);
auto static line_width = config::DEFAULT_LINE_WIDTH;

auto static kb = keyboard{};

auto static default_world_mut = config::DEFAULT_WORLD;
auto static world_ptr = ptr::nonnull_ptr_to(default_world_mut);

} // namespace state

auto get() -> renderer& {
    auto static lazy_static = renderer{};
    return lazy_static;
}

renderer::renderer() {
    // GLUT requires argc and argv to be passed to their init function,
    // which we don't want to forward.
    // Therefore, we create our own dummy values and forward those instead.
    static int dummy_argc = 1;
    static char dummy_arg[] = "";
    static char* dummy_argv[] = {dummy_arg};
    glutInit(&dummy_argc, dummy_argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(config::WIN_POS_X, config::WIN_POS_Y);
    glutInitWindowSize(config::WIN_WIDTH, config::WIN_HEIGHT);
    glutCreateWindow(config::WIN_TITLE);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutDisplayFunc(render);
    glutReshapeFunc(resize);
    glutTimerFunc(config::RENDER_TICK_MILLIS, update_camera, 0);
    glutKeyboardFunc(key_down);
    glutKeyboardUpFunc(key_up);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(
        config::DEFAULT_BG_COLOR.r,
        config::DEFAULT_BG_COLOR.g,
        config::DEFAULT_BG_COLOR.b,
        config::DEFAULT_BG_COLOR.a
    );
    glPolygonMode(GL_FRONT, config::DEFAULT_POLYGON_MODE);
    glLineWidth(config::DEFAULT_LINE_WIDTH);
    display_info();
}

auto renderer::set_world(world& world) noexcept -> renderer& {
    state::world_ptr = &world;
    return *this;
}

[[noreturn]]
auto renderer::run() noexcept -> void {
    glutMainLoop();
    __builtin_unreachable(); // glutMainLoop is noreturn,
                             // but isn't marked as such.
}

auto render() noexcept -> void {
    auto const& camera = state::world_ptr->camera;
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // TODO: Use glm transforms instead of GLUT to avoid casting.

    // glMultMatrixf(
    //     glm::value_ptr(
    //         glm::lookAt(camera.pos, camera.lookat, camera.up)
    //     )
    // );
    // glTranslatef(-camera.pos.x, -camera.pos.y, -camera.pos.z);

#   define DBL(x) static_cast<double>(x)
    gluLookAt(
        DBL(camera.pos.x),    DBL(camera.pos.y),    DBL(camera.pos.z),
        DBL(camera.lookat.x), DBL(camera.lookat.y), DBL(camera.lookat.z),
        DBL(camera.up.x),     DBL(camera.up.y),     DBL(camera.up.z)
    );
#   undef DBL

    glPolygonMode(GL_FRONT, state::polygon_mode);
    glLineWidth(state::line_width);
    if (state::enable_axis) {
        render_axis();
    }
    render_group(state::world_ptr->root);
    glutSwapBuffers();
}

auto update_camera(int) noexcept -> void {
    using enum config::kb_keys;

    auto const& kb = state::kb;

    if (not kb.pressed(KEY_ROTATE_UP)
        and not kb.pressed(KEY_ROTATE_LEFT)
        and not kb.pressed(KEY_ROTATE_DOWN)
        and not kb.pressed(KEY_ROTATE_RIGHT)
        and not kb.pressed(KEY_ZOOM_IN)
        and not kb.pressed(KEY_ZOOM_OUT)
    ) {
        // Skip camera update if no key is presssed.
        glutTimerFunc(config::RENDER_TICK_MILLIS, update_camera, 0);
        return;
    }

    auto& camera = state::world_ptr->camera;
    auto relative_pos = camera.pos - camera.lookat;

    ::util::cartesian_to_spherical_inplace(relative_pos);

    // Rotate left/right.
    if (kb.pressed(KEY_ROTATE_LEFT) and not kb.pressed(KEY_ROTATE_RIGHT)) {
        relative_pos[2] -= config::CAM_ROTATE_STEP;
    } else if (kb.pressed(KEY_ROTATE_RIGHT) and not kb.pressed(KEY_ROTATE_LEFT)) {
        relative_pos[2] += config::CAM_ROTATE_STEP;
    }

    // Rotate up/down.
    if (kb.pressed(KEY_ROTATE_UP) and not kb.pressed(KEY_ROTATE_DOWN)) {
        relative_pos[1] = std::max(
            config::CAM_VERT_ANGLE_MIN,
            relative_pos[1] - config::CAM_ROTATE_STEP
        );
    } else if (kb.pressed(KEY_ROTATE_DOWN) and not kb.pressed(KEY_ROTATE_UP)) {
        relative_pos[1] = std::min(
            config::CAM_VERT_ANGLE_MAX,
            relative_pos[1] + config::CAM_ROTATE_STEP
        );
    }

    // Zoom in/out.
    if (kb.pressed(KEY_ZOOM_IN) and not kb.pressed(KEY_ZOOM_OUT)) {
        relative_pos[0] = std::max(
            config::CAM_ZOOM_MAX,
            relative_pos[0] - config::CAM_ZOOM_STEP
        );
    } else if (kb.pressed(KEY_ZOOM_OUT) and not kb.pressed(KEY_ZOOM_IN)) {
        relative_pos[0] = std::min(
            config::CAM_ZOOM_MIN,
            relative_pos[0] + config::CAM_ZOOM_STEP
        );
    }

    ::util::spherical_to_cartesian_inplace(relative_pos);

    camera.pos = relative_pos + camera.lookat;

    glutPostRedisplay();
    glutTimerFunc(config::RENDER_TICK_MILLIS, update_camera, 0);
}

auto render_axis() noexcept -> void {
    glBegin(GL_LINES);

    // x axis.
    glColor3fv(glm::value_ptr(config::AXIS_COLOR[0]));
    glVertex3f(-config::X_AXIS_HALF_LEN, 0.f, 0.f);
    glVertex3f(config::X_AXIS_HALF_LEN, 0.f, 0.f);

    // y axis.
    glColor3fv(glm::value_ptr(config::AXIS_COLOR[1]));
    glVertex3f(0.f, -config::Y_AXIS_HALF_LEN, 0.f);
    glVertex3f(0.f, config::Y_AXIS_HALF_LEN, 0.f);

    // z axis.
    glColor3fv(glm::value_ptr(config::AXIS_COLOR[2]));
    glVertex3f(0.f, 0.f, -config::Z_AXIS_HALF_LEN);
    glVertex3f(0.f, 0.f, config::Z_AXIS_HALF_LEN);

    glEnd();

    glColor3fv(glm::value_ptr(config::DEFAULT_FG_COLOR));
}

// TODO: Implement non-recursively.
auto render_group(group const& root) noexcept -> void {
    glPushMatrix();

    for (auto const& transform : root.transforms) {
        switch (transform.kind) {
            using enum transform::kind_t;
            case translate:
                glTranslatef(
                    transform.translate.x,
                    transform.translate.y,
                    transform.translate.z
                );
                break;
            case rotate:
                glRotatef(
                    transform.rotate[0],
                    transform.rotate[1],
                    transform.rotate[2],
                    transform.rotate[3]
                );
                break;
            case scale:
                glScalef(
                    transform.scale.x,
                    transform.scale.y,
                    transform.scale.z
                );
                break;
        }
    }

    for (auto const& model : root.models) {
        auto const* i = model.coords.data();
        auto const* const end = i + model.coords.size();
        glBegin(GL_TRIANGLES);
        for ( ; i != end; i += 3) {
            glVertex3fv(i);
        }
        glEnd();
    }

    for (auto const& child_node : root.children) {
        render_group(child_node);
    }

    glPopMatrix();
}

auto resize(int const width, int height) noexcept -> void {
    // Prevent a divide by zero when window is too short.
    if (height == 0) {
        height = 1;
    }

    auto const& camera_proj = state::world_ptr->camera.projection;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(
        static_cast<double>(camera_proj[0]),
        static_cast<double>(width) / static_cast<double>(height),
        static_cast<double>(camera_proj[1]),
        static_cast<double>(camera_proj[2])
    );
    glMatrixMode(GL_MODELVIEW);
}

auto key_down(unsigned char const key, int, int) noexcept -> void {
    state::kb.press(key);
    switch (key) {
        using enum config::kb_keys;
        case KEY_TOGGLE_AXIS:
            state::enable_axis = not state::enable_axis;
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
                    __builtin_unreachable();
            }
            break;
        case KEY_THINNER_LINES:
            state::line_width = std::max(
                state::line_width - config::LINE_WIDTH_STEP,
                config::LINE_WIDTH_MIN
            );
            break;
        case KEY_THICKER_LINES:
            state::line_width = std::min(
                state::line_width + config::LINE_WIDTH_STEP,
                config::LINE_WIDTH_MAX
            );
            break;
        default:
            break;
    }
}

auto key_up(unsigned char key, int, int) noexcept -> void {
    if (std::isalpha(key)) {
        key = static_cast<unsigned char>(tolower(key));
    }
    state::kb.release(key);
}

auto display_info() -> void {
    // reinterpret_cast is needed to silence some fmt + unsigned char warnings.
    spdlog::info(
        "vendor: {}.",
        reinterpret_cast<char const*>(glGetString(GL_VENDOR))
    );
    spdlog::info(
        "renderer: {}.",
        reinterpret_cast<char const*>(glGetString(GL_RENDERER))
    );
    spdlog::info(
        "version: {}.",
        reinterpret_cast<char const*>(glGetString(GL_VERSION))
    );
}

} // namespace engine::render
