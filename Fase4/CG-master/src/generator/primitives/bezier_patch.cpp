#include "generator/primitives/bezier_patch.hpp"

#include "util/try.hpp"

#include <array>
#include <fstream>
#include <new>
#include <ranges>
#include <stdexcept>
#include <utility>

auto normals_bezier = std::vector<glm::vec3>{};
auto texcoord_bezier = std::vector<glm::vec2>{};

namespace generator {

using namespace brief_int;

struct BezierPatch {
    std::vector<std::array<usize, 16>> indices;
    std::vector<glm::vec3> ctrl_points;
};

void multMatrixVector(float *m, float *v, float *res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }

}

void multMatrixMatrix(float* m1, float* m2, float* res) {
    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        res[j+4] = 0;
        res[j+8] = 0;
        res[j+12] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += m1[j + 4 * k] * m2[k];
            res[j+4] += m1[j + 4 * k] * m2[4 + k];
            res[j+8] += m1[j + 4 * k] * m2[8 + k];
            res[j+12] += m1[j + 4 * k] * m2[12 + k];
        }
    }
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

glm::vec3 calcBenzierCurve(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    glm::vec3 res;
    float A[3][4];
    float m[4][4] = { {-1,  3, -3, 1},
                      { 3, -6,  3, 0},
                      {-3,  3,  0,  0},
                      { 1,  0,  0,  0} };

    for (int i = 0; i < 3; i++) {
        float pp[4] = { p0[i],p1[i],p2[i],p3[i] };
        multMatrixVector(*m, pp, A[i]);

        res[i] = t * t * t * A[i][0] + t * t * A[i][1] + t * A[i][2] + A[i][3];
    }
    return res;
}

void derivsBenzierCurve(float u, float v, float* x, float* y, float* z, float* n) {
    float U[4] = {u*u*u,u*u,u,1};
    float dU[4] = {3*u*u,2*u,1,0};
    float V[4] = {v*v*v,v*v,v,1};
    float dV[4] = {3*v*v,2*v,1,0};

    float vecux[4],vecuy[4],vecuz[4];
    float vecvx[4],vecvy[4],vecvz[4];

    float realu[3],realv[3];

    multMatrixVector(x,dU,vecux);
    multMatrixVector(y,dU,vecuy);
    multMatrixVector(z,dU,vecuz);

    multMatrixVector(x,U,vecvx);
    multMatrixVector(y,U,vecvy);
    multMatrixVector(z,U,vecvz);

    realu[0] = vecux[0]*V[0] + vecux[1]*V[1] + vecux[2]*V[2] + vecux[3]*V[3];
    realu[1] = vecuy[0]*V[0] + vecuy[1]*V[1] + vecuy[2]*V[2] + vecuy[3]*V[3];
    realu[2] = vecuz[0]*V[0] + vecuz[1]*V[1] + vecuz[2]*V[2] + vecuz[3]*V[3];

    realv[0] = vecvx[0]*dV[0] + vecvx[1]*dV[1] + vecvx[2]*dV[2] + vecvx[3]*dV[3];
    realv[1] = vecvy[0]*dV[0] + vecvy[1]*dV[1] + vecvy[2]*dV[2] + vecvy[3]*dV[3];
    realv[2] = vecvz[0]*dV[0] + vecvz[1]*dV[1] + vecvz[2]*dV[2] + vecvz[3]*dV[3];

    cross(realu,realv,n);
    normalize(n);
}

auto static parse_patch_file(std::ifstream& patch_file) noexcept
    -> cpp::result<BezierPatch, GeneratorErr>;

auto generate_bezier_patch(
    std::ifstream& patch_input_file,
    u32 const tesselation
) noexcept -> cpp::result<std::vector<glm::vec3>, GeneratorErr>
try {
    auto const& bezier_patch = TRY_RESULT(parse_patch_file(patch_input_file));
    glm::vec3 p0, p1, p2, p3;
    glm::vec3 pp0, pp1, pp2, pp3;
    float tess = tesselation;
    float step = 1 / tess;
    glm::vec3 pa, pb, pc, pd;
    float pan[3], pbn[3], pcn[3], pdn[3];

    auto points = bezier_patch.ctrl_points;
    auto vertices = std::vector<glm::vec3>{};

    float tmpx[4][4],tmpy[4][4],tmpz[4][4];
    float resx[4][4],resy[4][4],resz[4][4];
    float m[4][4] = { {-1,  3, -3, 1},
                      { 3, -6,  3, 0},
                      {-3,  3,  0,  0},
                      { 1,  0,  0,  0} };

    for (auto const& patch : bezier_patch.indices)
    {
        float x[4][4] = { {points[patch[0]].x, points[patch[1]].x, points[patch[2]].x, points[patch[3]].x},
                      { points[patch[4]].x, points[patch[5]].x, points[patch[6]].x, points[patch[7]].x},
                      {points[patch[8]].x, points[patch[9]].x, points[patch[10]].x, points[patch[11]].x},
                      { points[patch[12]].x, points[patch[13]].x, points[patch[14]].x, points[patch[15]].x} };
        float y[4][4] = { {points[patch[0]].y, points[patch[1]].y, points[patch[2]].y, points[patch[3]].y},
                      { points[patch[4]].y, points[patch[5]].y, points[patch[6]].y, points[patch[7]].y},
                      {points[patch[8]].y, points[patch[9]].y, points[patch[10]].y, points[patch[11]].y},
                      { points[patch[12]].y, points[patch[13]].y, points[patch[14]].y, points[patch[15]].y} };
        float z[4][4] = { {points[patch[0]].z, points[patch[1]].z, points[patch[2]].z, points[patch[3]].z},
                      { points[patch[4]].z, points[patch[5]].z, points[patch[6]].z, points[patch[7]].z},
                      {points[patch[8]].z, points[patch[9]].z, points[patch[10]].z, points[patch[11]].z},
                      { points[patch[12]].z, points[patch[13]].z, points[patch[14]].z, points[patch[15]].z} };

        multMatrixMatrix(*m,*x,*tmpx);
        multMatrixMatrix(*m,*y,*tmpy);
        multMatrixMatrix(*m,*z,*tmpz);

        multMatrixMatrix(*tmpx,*m,*resx);
        multMatrixMatrix(*tmpy,*m,*resy);
        multMatrixMatrix(*tmpz,*m,*resz);


    for (float i = 0; i < 1; i += step)
    {
        p0 = calcBenzierCurve(i, points[patch[0]], points[patch[1]], points[patch[2]], points[patch[3]]);
        p1 = calcBenzierCurve(i, points[patch[4]], points[patch[5]], points[patch[6]], points[patch[7]]);
        p2 = calcBenzierCurve(i, points[patch[8]], points[patch[9]], points[patch[10]], points[patch[11]]);
        p3 = calcBenzierCurve(i, points[patch[12]], points[patch[13]], points[patch[14]], points[patch[15]]);

        pp0 = calcBenzierCurve(i + step, points[patch[0]], points[patch[1]], points[patch[2]], points[patch[3]]);
        pp1 = calcBenzierCurve(i + step, points[patch[4]], points[patch[5]], points[patch[6]], points[patch[7]]);
        pp2 = calcBenzierCurve(i + step, points[patch[8]], points[patch[9]], points[patch[10]], points[patch[11]]);
        pp3 = calcBenzierCurve(i + step, points[patch[12]], points[patch[13]], points[patch[14]], points[patch[15]]);

        for (float j = 0; j < 1; j += step)
        {
            pa = calcBenzierCurve(j, p0, p1, p2, p3);
            pb = calcBenzierCurve(j, pp0, pp1, pp2, pp3);
            pc = calcBenzierCurve(j + step, p0, p1, p2, p3);
            pd = calcBenzierCurve(j + step, pp0, pp1, pp2, pp3);

            derivsBenzierCurve(i,j,*resx,*resy,*resz,pan);
            derivsBenzierCurve(i + step,j,*resx,*resy,*resz,pbn);
            derivsBenzierCurve(i,j + step,*resx,*resy,*resz,pcn);
            derivsBenzierCurve(i + step,j + step,*resx,*resy,*resz,pdn);

            vertices.emplace_back(pa[0],pa[1],pa[2]);
            vertices.emplace_back(pb[0], pb[1], pb[2]);
            vertices.emplace_back(pc[0], pc[1], pc[2]);

            vertices.emplace_back(pb[0], pb[1], pb[2]);
            vertices.emplace_back(pd[0], pd[1], pd[2]);
            vertices.emplace_back(pc[0], pc[1], pc[2]);

            normals_bezier.emplace_back(pan[0],pan[1],pan[2]);
            normals_bezier.emplace_back(pbn[0], pbn[1], pbn[2]);
            normals_bezier.emplace_back(pcn[0], pcn[1], pcn[2]);

            normals_bezier.emplace_back(pbn[0], pbn[1], pbn[2]);
            normals_bezier.emplace_back(pdn[0], pdn[1], pdn[2]);
            normals_bezier.emplace_back(pcn[0], pcn[1], pcn[2]);

            texcoord_bezier.emplace_back(i,j);
            texcoord_bezier.emplace_back(i + step,j);
            texcoord_bezier.emplace_back(i,j + step);

            texcoord_bezier.emplace_back(i + step,j);
            texcoord_bezier.emplace_back(i + step,j + step);
            texcoord_bezier.emplace_back(i,j + step);
        }
    }
    }
    return vertices;
} catch (std::bad_alloc const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
} catch (std::length_error const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
}

auto generate_and_print_bezier_patch(
    std::ifstream& patch_input_file,
    u32 const tesselation,
    fmt::ostream& output_file
) noexcept -> cpp::result<void, GeneratorErr>
try {
    auto const& vertices = TRY_RESULT(
        generate_bezier_patch(patch_input_file, tesselation)
    );
    output_file.print("{}\n", vertices.size());
    for (auto const& vertex : vertices) {
        output_file.print("{} {} {}\n", vertex.x, vertex.y, vertex.z);
    }
    output_file.print("\n");
    for (auto const& vertex : normals_bezier) {
        output_file.print("{} {} {}\n", vertex.x, vertex.y, vertex.z);
    }
    output_file.print("\n");
    for (auto const& vertex : texcoord_bezier) {
        output_file.print("{} {}\n", vertex.x, vertex.y);
    }
    return {};
} catch (...) {
    return cpp::fail(GeneratorErr::IO_ERR);
}

#define CHECK_FILE_STATUS(file_const_ref)                                      \
    do {                                                                       \
        if (file_const_ref.bad()) {                                            \
            return cpp::fail(GeneratorErr::IO_ERR);                            \
        }                                                                      \
        if (file_const_ref.fail()) {                                           \
            return cpp::fail(GeneratorErr::MALFORMED_NUM);                     \
        }                                                                      \
    } while (0)

auto static parse_patch_file(std::ifstream& patch_file) noexcept
    -> cpp::result<BezierPatch, GeneratorErr>
try {
    using namespace brief_int::literals;

    usize num_patches;
    patch_file >> num_patches;
    CHECK_FILE_STATUS(patch_file);
    if (num_patches < 1) {
        return cpp::fail(GeneratorErr::BEZIER_LT_ONE_PATCH);
    }

    auto indices = std::vector<std::array<usize, 16>>{};
    indices.reserve(num_patches);

    auto line_buffer = std::string{};

    for (auto i = 0_uz; i < num_patches; ++i) {
        using std::ranges::views::take;
        std::array<usize, 16> patch;
        for (auto& e : patch | take(15)) {
            patch_file >> e;
            CHECK_FILE_STATUS(patch_file);
            patch_file.ignore(1, ',');
        }
        patch_file >> patch.back();
        CHECK_FILE_STATUS(patch_file);
        indices.push_back(std::move(patch));
    }

    usize num_ctrl_points;
    patch_file >> num_ctrl_points;
    CHECK_FILE_STATUS(patch_file);
    if (num_ctrl_points < 1) {
        return cpp::fail(GeneratorErr::BEZIER_LT_ONE_CTRL_POINT);
    }

    auto ctrl_points = std::vector<glm::vec3>{};
    ctrl_points.reserve(num_ctrl_points);

    for (auto i = 0_uz; i < num_ctrl_points; ++i) {
        using idx_t = glm::vec3::length_type;
        glm::vec3 ctrl_point;
        for (idx_t i = 0; i < 2; ++i) {
            patch_file >> ctrl_point[i];
            CHECK_FILE_STATUS(patch_file);
            patch_file.ignore(1, ',');
        }
        patch_file >> ctrl_point[2];
        CHECK_FILE_STATUS(patch_file);
        ctrl_points.push_back(std::move(ctrl_point));
    }

    return BezierPatch {
        .indices = std::move(indices),
        .ctrl_points = std::move(ctrl_points),
    };

} catch (std::bad_alloc const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
} catch (std::length_error const&) {
    return cpp::fail(GeneratorErr::NO_MEM);
}

} // namespace generator
