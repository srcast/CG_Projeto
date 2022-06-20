#include "generator/primitives/bezier_patch.hpp"

#include "util/try.hpp"

#include <array>
#include <fstream>
#include <new>
#include <ranges>
#include <stdexcept>
#include <utility>

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

        // Compute pos = T * A
        res[i] = t * t * t * A[i][0] + t * t * A[i][1] + t * A[i][2] + A[i][3];
    }
    return res;
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

    auto points = bezier_patch.ctrl_points;
    auto vertices = std::vector<glm::vec3>{};


    for (auto const& patch : bezier_patch.indices)
    {

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

            vertices.emplace_back(pa[0],pa[1],pa[2]);
            vertices.emplace_back(pb[0], pb[1], pb[2]);
            vertices.emplace_back(pc[0], pc[1], pc[2]);

            vertices.emplace_back(pb[0], pb[1], pb[2]);
            vertices.emplace_back(pd[0], pd[1], pd[2]);
            vertices.emplace_back(pc[0], pc[1], pc[2]);
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
