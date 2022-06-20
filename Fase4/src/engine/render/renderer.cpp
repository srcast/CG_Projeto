#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <GL/glew.h>
#include "engine/render/renderer.hpp"

#include "engine/config.hpp"
#include "engine/render/io_events.hpp"
#include "engine/render/render.hpp"
#include "engine/render/state.hpp"

#include <spdlog/spdlog.h>



#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

double frames = 0;
double timebase;
double fps;

namespace engine::render {

auto framerate () -> void; 

auto static display_info() -> void;

auto get() -> Renderer& {
    auto static lazy_static = Renderer{};
    return lazy_static;
}




auto static bufferVBOs(Group const& root) noexcept -> void {
    for (auto const& model : root.models) {
        std::vector<float> BufferModel;
        for (auto const& vertex : model.vertices) {
            BufferModel.push_back(vertex[0]);
            BufferModel.push_back(vertex[1]);
            BufferModel.push_back(vertex[2]);
        }
        state::buffers.push_back(BufferModel);
    }
    for (auto const& child_node : root.children) {
        bufferVBOs(child_node);
    }
}

Renderer::Renderer() {
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
    glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutDisplayFunc(render);
    glutReshapeFunc(resize);
    glutTimerFunc(config::RENDER_TICK_MILLIS, [](int) { update_camera(); }, 0);
    glutKeyboardFunc(key_down);
    glutKeyboardUpFunc(key_up);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glewInit();
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
	//glEnable(GL_RESCALE_NORMAL);
    glEnableClientState(GL_VERTEX_ARRAY);

    glClearColor(
        config::DEFAULT_BG_COLOR.r,
        config::DEFAULT_BG_COLOR.g,
        config::DEFAULT_BG_COLOR.b,
        config::DEFAULT_BG_COLOR.a
    );
    glPolygonMode(GL_FRONT, config::DEFAULT_POLYGON_MODE);
    glLineWidth(config::DEFAULT_LINE_WIDTH);
    framerate();
    display_info();
}

auto Renderer::set_world(World& world) -> Renderer& {
    if (auto* const world_ptr = &world;
        world_ptr != state::world_ptr
    ) {
        state::world_ptr = world_ptr;
        bufferVBOs(state::world_ptr->root);

        int i = 0;

        glEnableClientState(GL_VERTEX_ARRAY);
        //glEnableClientState(GL_NORMAL_ARRAY);
        //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        //glEnable(GL_TEXTURE_2D);

        //TALVEZ FAZER AQUI A DEFINIÇÃO DAS LUZES???

        //float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
        
        glGenBuffers(500,state::bind[0]);
        int tamanho = static_cast<int> (state::buffers.size());

        for(int i = 0; i < tamanho;i++){
            glBindBuffer(GL_ARRAY_BUFFER, state::bind[0][i]);
            glBufferData(GL_ARRAY_BUFFER,sizeof(float) * state::buffers[i].size(),state::buffers[i].data(),GL_STATIC_DRAW);

            //glGenBuffers(500,state::bind[1]);
            //glBindBuffer(GL_ARRAY_BUFFER, state::bind[1][i]);
            //glBufferData(GL_ARRAY_BUFFER,sizeof(float) * NORMAL DO MODELO.size(), NORMAL DO MODELO.data() ,GL_STATIC_DRAW);

            //glGenBuffers(500,state::bind[2]);
            //glBindBuffer(GL_ARRAY_BUFFER, state::bind[2][i]);
            //glBufferData(GL_ARRAY_BUFFER,sizeof(float) * TEXT DO MODELO.size(), TEXT DO MODELO.data() ,GL_STATIC_DRAW);
        }

    }
    return *this;
}

auto Renderer::set_camera(Camera& camera) -> Renderer& {
    state::camera_ptr = &camera;
    return *this;
}
/*
auto Renderer::set_lights(Lights& l) -> Renderer& {
    int luzes = 0 ;
    GLfloat white[4] = { 1.0, 1.0, 1.0, 1.0 };
    for (auto const& luz : l) {
        glLightfv(GL_LIGHT0+luzes, GL_DIFFUSE, white);
        glLightfv(GL_LIGHT0+luzes, GL_SPECULAR, white);

        if (type == point){
            glLightfv(GL_LIGHT0,GL_POSITION,luz->pos);
        }
        if (type == direct){
            glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,luz->dirct);
        }
        if (type == spot){
            glLightfv(GL_LIGHT0,GL_POSITION,luz->pos);
            glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,luz->dirct);
            glLightfv(GL_LIGHT0,GL_SPOT_CUTOFF,luz->cutoff);
        } 
    }
}
*/

auto Renderer::run() noexcept -> void {
    glutMainLoop();
}

auto framerate () -> void {

    frames++;
    double time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        fps = frames * 1000.0 / (time - timebase);
        timebase = time;
        frames = 0;

    }
    spdlog::info(
        "time: {}.",
        time
    );
    spdlog::info(
        "FPS: {}.",
        fps
    );
}

auto static display_info() -> void {
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
