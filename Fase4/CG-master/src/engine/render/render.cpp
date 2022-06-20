// For some reason this needs to be included first.
#include <GL/glew.h>

#include "engine/render/render.hpp"

#include "engine/config.hpp"
#include "engine/render/layout/world/group/group.hpp"
#include "engine/render/layout/world/group/model.hpp"
#include "engine/render/layout/world/group/transform/transform.hpp"
#include "engine/render/layout/world/group/transform/rotate.hpp"
#include "engine/render/state.hpp"
#include "generator/primitives/box.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include "util/overload.hpp"

int iii = 0;

namespace engine::render {

auto static render_axis() noexcept -> void;
auto static render_lookat_indicator() noexcept -> void;
auto static render_group(Group const& root) noexcept -> void;


auto render() noexcept -> void {
    auto const& camera = *state::camera_ptr;
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
    if (state::enable_lookat_indicator) {
        render_lookat_indicator();
    }
    render_group(state::world_ptr->root);
    iii = 0;
    glutSwapBuffers();
}

auto resize(int const width, int height) noexcept -> void {
    // Prevent a divide by zero when window is too short.
    if (height == 0) {
        height = 1;
    }

    auto const& camera_proj = state::camera_ptr->projection;

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

auto static render_axis() noexcept -> void {
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

auto static render_lookat_indicator() noexcept -> void {
    auto static const lookat_model = ::generator::generate_box(0.5f, 1).value();
    glColor3fv(glm::value_ptr(config::LOOKAT_INDICATOR_COLOR));
    auto const& translate = state::camera_ptr->lookat;
    glPushMatrix();
    glTranslatef(translate.x, translate.y, translate.z);
    glBegin(GL_TRIANGLES);
    for (auto const& vertex : lookat_model) {
        glVertex3fv(glm::value_ptr(vertex));
    }
    glEnd();
    glPopMatrix();
    glColor3fv(glm::value_ptr(config::DEFAULT_FG_COLOR));
}

void buildRotMatrix(float *x, float *y, float *z, float *m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void cross(float *a, float *b, float *res) {

    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(float *a) {

    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

void multMatrixVector(float *m, float *v, float *res) {
    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }
}

void getCatmullRomPoint(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float *pos, float *deriv) {
    // catmull-rom matrix
    float m[4][4] = {    {-0.5f,  1.5f, -1.5f,  0.5f},
                        { 1.0f, -2.5f,  2.0f, -0.5f},
                        {-0.5f,  0.0f,  0.5f,  0.0f},
                        { 0.0f,  1.0f,  0.0f,  0.0f}};
            
    // Compute A = M * P
    float A[3][4];
    for (int i = 0; i < 3; i++) {
        float pp[4] = { p0[i],p1[i],p2[i],p3[i] };
        multMatrixVector(*m, pp, A[i]);
    
    // Compute pos = T * A
        pos[i] = t * t * t * A[i][0] + t * t * A[i][1] + t * A[i][2] + A[i][3];

    // compute deriv = T' * A
        deriv[i] = 3 * t * t * A[i][0] + 2 * t * A[i][1] + A[i][2];
    }
}

// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt,std::vector<glm::vec3> p, float *pos, float *deriv) {
    float t = gt * p.size(); // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4]; 
    indices[0] = (index + p.size()-1)%p.size();    
    indices[1] = (indices[0]+1)%p.size();
    indices[2] = (indices[1]+1)%p.size(); 
    indices[3] = (indices[2]+1)%p.size();

    getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], pos, deriv);
}

void renderCatmullRomCurve(std::vector<glm::vec3> points) {
    float pos[3];
    float deriv[3];
// draw curve using line segments with GL_LINE_LOOP
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 100; i += 1) {
        getGlobalCatmullRomPoint(i*0.01,points,pos, deriv);
        glVertex3f(pos[0], pos[1], pos[2]);
    }
    glEnd();
}

// TODO: Implement non-recursively.
auto static render_group(Group const& root) noexcept -> void {
    glPushMatrix();

    for (auto const& transform : root.transforms) {
        std::visit(util::overload {
            [](Translate const& translate) {
                std::visit(util::overload {
                    [](StaticTranslate const& static_translate) {
                        glTranslatef(
                            static_translate.xyz.x,
                            static_translate.xyz.y,
                            static_translate.xyz.z
                        );
                    },
                    [](DynamicTranslate const& dynamic_translate) {
                        float pos[3];
                        float deriv[3];
                        static float yo[3] = { 0, 1, 0 };
                        float z[3];
                        float m[16];

                        float gt = glutGet(GLUT_ELAPSED_TIME);
                        float timer = dynamic_translate.time;
                        float realt = gt / (timer * 1000);

                        renderCatmullRomCurve(dynamic_translate.points);
                        getGlobalCatmullRomPoint(realt,dynamic_translate.points, pos, deriv);

                        glTranslatef(pos[0], pos[1], pos[2]);
                        if(dynamic_translate.align){ // if assign = True 
                            normalize(deriv);
                            cross(deriv, yo, z);
                            normalize(z);
                            cross(z, deriv, yo);
                            normalize(yo);
                            buildRotMatrix(deriv, yo, z, m);
                            glMultMatrixf(m);
                        }
                    }
                }, translate);
            },
            [](Rotate const& rotate) {
                switch (rotate.kind) {
                    using enum Rotate::Kind;

                    case Angle:
                        glRotatef(
                            rotate.rotate[0],
                            rotate.rotate[1],
                            rotate.rotate[2],
                            rotate.rotate[3]
                        );
                        break;
                    case Time: {
                        auto const gt = glutGet(GLUT_ELAPSED_TIME);
                        auto const timer = rotate.rotate[0];
                        auto const realt = gt / (timer * 1000);
                        glRotatef(
                            360 * realt,
                            rotate.rotate[1],
                            rotate.rotate[2],
                            rotate.rotate[3]
                        );
                        break;
                    }
                }
            },
            [](Scale const& scale) {
                glScalef(scale.x, scale.y, scale.z);
            }
        }, transform);
    }

    for (auto const& model : root.models) {

        //glBindTexture(GL_TEXTURE_2D,ID DA TEXT DO MODEL);
        /*
        glMaterialfv(GL_FRONT, GL_AMBIENT, model->ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, model->difuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, model->specular);
        glMaterialf(GL_FRONT, GL_SHININESS, model->shininess);
        glMaterialf(GL_FRONT, GL_EMISSION, model->emession);

        */


        glBindBuffer(GL_ARRAY_BUFFER, state::bind[0][iii]);
        glVertexPointer(3,GL_FLOAT,0,0);

        //Normal
        //glBindBuffer(GL_ARRAY_BUFFER, state::bind[1][iii]);
        //glNormalPointer(GL_FLOAT,0,0);

        //Textura
        //if(modelo tem textura){
        //      glBindBuffer(GL_ARRAY_BUFFER, state::bind[2][iii]);
        //      glTexCoordPointer(2,GL_FLOAT,0,0);}


        glDrawArrays(GL_TRIANGLES,0,state::buffers[iii].size()/3);
        iii++;
        //glBindTexture(GL_TEXTURE_2D,0);
    }
    for (auto const& child_node : root.children) {
        render_group(child_node);
    }

    glPopMatrix();
}

} // namespace engine::render
