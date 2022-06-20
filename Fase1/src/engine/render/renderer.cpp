#include "engine/render/renderer.hpp"

#include "engine/config.hpp"
#include "engine/render/keyboard.hpp"
#include "engine/render/world/world.hpp"
#include "util/pretty_print.hpp"

#include <GL/freeglut.h>
#include <fmt/core.h>

namespace engine::render {

auto render() noexcept -> void;
auto update_camera(int) noexcept -> void;
auto render_group(group const& root) noexcept -> void;
auto resize(int width, int height) noexcept -> void;
auto display_info() noexcept -> void;

namespace state {
    // Will eventually allow "reverting" to default world to undo changes.
    auto aspect_ratio = config::DEFAULT_ASPECT_RATIO;
    auto default_world_mut = config::DEFAULT_WORLD;
    auto* world_ptr = &default_world_mut;
    auto kb = keyboard{};
} // namespace state

auto launch() noexcept -> renderer& {
    auto static lazy_static = renderer{};
    return lazy_static;
}

renderer::renderer() noexcept {
    // GLUT requires argc and argv to be passed to their init function,
    // which we don't want to forward.
    // Therefore, we create our own dummy values and forward those instead.
    static int dummy_argc = 1;
    static char dummy_arg[] = "";
    static char* dummy_argv[] = {dummy_arg};
    glutInit(&dummy_argc, dummy_argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(
        config::DEFAULT_WIN_POS_X,
        config::DEFAULT_WIN_POS_Y
    );
    glutInitWindowSize(config::DEFAULT_WIN_WIDTH, config::DEFAULT_WIN_HEIGHT);
    glutCreateWindow(config::WIN_TITLE);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutDisplayFunc(render);
    glutReshapeFunc(resize);
    glutTimerFunc(config::RENDER_TICK_MILLIS, update_camera, 0);
    glutKeyboardFunc([](unsigned char const key, int, int) {
        switch (key) {
            using enum config::camera_keys;
            case KEY_MOVE_UP:
            case KEY_MOVE_LEFT:
            case KEY_MOVE_DOWN:
            case KEY_MOVE_RIGHT:
                state::kb.press(key);
                glutPostRedisplay();
                break;
            default:
                break;
        }
    });
    glutKeyboardUpFunc([](unsigned char const key, int, int) {
        switch (key) {
            using enum config::camera_keys;
            case KEY_MOVE_UP:
            case KEY_MOVE_LEFT:
            case KEY_MOVE_DOWN:
            case KEY_MOVE_RIGHT:
                state::kb.release(key);
                glutPostRedisplay();
                break;
            default:
                break;
        }
    });

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(
        config::DEFAULT_BG_COLOR.r,
        config::DEFAULT_BG_COLOR.g,
        config::DEFAULT_BG_COLOR.b,
        config::DEFAULT_BG_COLOR.a
    );

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
    gluLookAt(
        camera.pos.x,    camera.pos.y,    camera.pos.z,
        camera.lookat.x, camera.lookat.y, camera.lookat.z,
        camera.up.x,     camera.up.y,     camera.up.z
    );
    glPolygonMode(GL_FRONT, GL_LINE);
    render_group(state::world_ptr->root);
    glutSwapBuffers();
}

// TODO: implement rotation and zoom.
auto update_camera(int) noexcept -> void {
    using state::kb;
    using enum config::camera_keys;

    auto& camera_pos = state::world_ptr->camera.pos;

    if (not (kb.pressed(KEY_MOVE_UP) and kb.pressed(KEY_MOVE_DOWN))) {
        // do not translate if both keys are pressed.
        if (kb.pressed(KEY_MOVE_UP)) {
            camera_pos.y += config::CAM_TRANSL_FACTOR;
        } else if (kb.pressed(KEY_MOVE_DOWN)) {
            camera_pos.y -= config::CAM_TRANSL_FACTOR;
        }
    }

    if (not (kb.pressed(KEY_MOVE_LEFT) and kb.pressed(KEY_MOVE_RIGHT))) {
        // do not translate if both keys are pressed.
        if (kb.pressed(KEY_MOVE_LEFT)) {
            camera_pos.x -= config::CAM_TRANSL_FACTOR;
        } else if (kb.pressed(KEY_MOVE_RIGHT)) {
            camera_pos.x += config::CAM_TRANSL_FACTOR;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(config::RENDER_TICK_MILLIS, update_camera, 0);
}

// TODO: Implement non-recursively.
auto render_group(group const& root) noexcept -> void {
    // TODO: Finish implementing transforms.
    // for (auto const& transform : root->transforms) {
    //     switch (transform.kind) {
    //         using enum decltype(transform::kind);
    //         case translate:
    //             glTranslatef(
    //                 transform.translate.x,
    //                 transform.translate.y,
    //                 transform.translate.z
    //             );
    //             break;
    //         case rotate:
    //             glRotatef(
    //                 transform.rotate[0],
    //                 transform.rotate[1],
    //                 transform.rotate[2],
    //                 transform.rotate[3]
    //             );
    //             break;
    //         case scale:
    //             glScalef(
    //                 transform.scale.x,
    //                 transform.scale.y,
    //                 transform.scale.z
    //             );
    //             break;
    //     }
    // }
    for (auto const& model : root.models) {
        float const* i = model.coords.data();
        float const* const end = i + model.coords.size();
        glBegin(GL_TRIANGLES);
        for ( ; i != end; i += 3) {
            glVertex3fv(i);
        }
        glEnd();
    }
    for (auto const& child_node : root.children) {
        render_group(child_node);
    }
}

auto resize(int const width, int height) noexcept -> void {
    // Prevent a divide by zero when window is too short.
    if (height == 0) {
        height = 1;
    }

    auto const& camera_proj = state::world_ptr->camera.projection;

    state::aspect_ratio
        = static_cast<double>(width) / static_cast<double>(height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(
        camera_proj[0], state::aspect_ratio, camera_proj[1], camera_proj[2]
    );
    glMatrixMode(GL_MODELVIEW);
}

auto display_info() noexcept -> void {
    using engine::config::prog_name;

    // reinterpret_cast is needed to silence some fmt + unsigned char warnings.
    pretty_print(
        "Vendor: {}\n",
        reinterpret_cast<char const*>(glGetString(GL_VENDOR))
    );
    pretty_print(
        "Renderer: {}\n",
        reinterpret_cast<char const*>(glGetString(GL_RENDERER))
    );
    pretty_print(
        "Version: {}\n",
        reinterpret_cast<char const*>(glGetString(GL_VERSION))
    );
}

} // namespace engine::render
