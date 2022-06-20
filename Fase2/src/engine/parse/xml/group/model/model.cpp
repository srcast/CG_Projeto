#define TINYOBJLOADER_IMPLEMENTATION

#include "engine/parse/xml/group/model/model.hpp"

#include "util/try.hpp"

#include <brief_int.hpp>
#include <fstream>
#include <new>
#include <stdexcept>
#include <string_view>
#include <tiny_obj_loader.h>
#include <utility>
#include <vector>

namespace engine::parse::xml {

auto parse_3d(char const* model_filename) noexcept
    -> cpp::result<render::model, parse_err>;

auto parse_obj(char const* model_filename) noexcept
    -> cpp::result<render::model, parse_err>;

auto parse_model(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<render::model, parse_err>
{
    auto const* const model_filename_attr = TRY_NULLABLE_OR(
        node->first_attribute("file"),
        return cpp::fail(parse_err::no_model_filename);
    );

    auto const* const model_filename = TRY_NULLABLE_OR(
        model_filename_attr->value(),
        return cpp::fail(parse_err::no_model_filename);
    );

    auto const model_filename_sized = std::string_view {
        model_filename,
        model_filename_attr->value_size(),
    };

    if (model_filename_sized.ends_with(".3d")) {
        return parse_3d(model_filename);
    } else if (model_filename_sized.ends_with(".obj")) {
        return parse_obj(model_filename);
    } else {
        return cpp::fail(parse_err::ambiguous_model_ext);
    }
}

auto parse_3d(char const* const model_filename) noexcept
    -> cpp::result<render::model, parse_err>
try {
    using namespace brief_int;
    using namespace brief_int::literals;

    auto model_file = std::ifstream{model_filename};
    if (not model_file) {
        return cpp::fail(parse_err::no_model_file);
    }

    usize num_vertices;
    model_file >> num_vertices;
    if (model_file.bad()) {
        return cpp::fail(parse_err::io_err);
    }
    if (model_file.fail()) {
        return cpp::fail(parse_err::malformed_num);
    }

    auto coords = std::vector<float>{};
    coords.reserve(num_vertices * 3_uz);

    float x, y, z;
    while (model_file >> x >> y >> z) {
        // read 3 by 3 to ensure there are no trailing coordinates.
        coords.push_back(x);
        coords.push_back(y);
        coords.push_back(z);
    }
    if (model_file.bad()) {
        return cpp::fail(parse_err::io_err);
    }
    if (model_file.eof()) {
        goto SUCCESS;
    }
    if (model_file.fail()) {
        return cpp::fail(parse_err::malformed_num);
    }

    SUCCESS:
    return render::model {
        .coords = std::move(coords)
    };

} catch (std::bad_alloc const&) {
    return cpp::fail(parse_err::no_mem);
} catch (std::length_error const&) {
    return cpp::fail(parse_err::no_mem);
}

auto parse_obj(char const* const model_filename) noexcept
    -> cpp::result<render::model, parse_err>
try {
    using namespace brief_int;
    using namespace brief_int::literals;

    auto model_file = std::ifstream{model_filename};
    if (not model_file) {
        return cpp::fail(parse_err::no_model_file);
    }

    auto attrib = tinyobj::attrib_t{};
    auto shapes = std::vector<tinyobj::shape_t>{};
    auto materials = std::vector<tinyobj::material_t>{};

    auto warn = std::string{};
    auto err = std::string{};

    if (not tinyobj::LoadObj(
            &attrib, &shapes, &materials, &err, &model_file
        ) or not err.empty()
    ) {
        return cpp::fail(parse_err::obj_loader_err);
    }

    auto coords = std::vector<float>{};

    auto num_vertices = 0_uz;
    for (auto const& shape : shapes) {
        num_vertices += shape.mesh.indices.size() * 3_uz;
    }

    coords.reserve(num_vertices);

    for (auto const& shape : shapes) {
        for (auto const& idx : shape.mesh.indices) {
            auto const vertex_idx = static_cast<usize>(idx.vertex_index) * 3_uz;
            coords.push_back(attrib.vertices[vertex_idx]);
            coords.push_back(attrib.vertices[vertex_idx + 1_uz]);
            coords.push_back(attrib.vertices[vertex_idx + 2_uz]);
        }
    }

    return render::model {
        .coords = std::move(coords)
    };

} catch (std::bad_alloc const&) {
    return cpp::fail(parse_err::no_mem);
} catch (std::length_error const&) {
    return cpp::fail(parse_err::no_mem);
}

} // namespace engine::parse::xml
