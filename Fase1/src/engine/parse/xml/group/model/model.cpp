#include "engine/parse/xml/group/model/model.hpp"

#include "util/try.hpp"

#include <brief_int.hpp>
#include <fstream>
#include <new>
#include <stdexcept>
#include <utility>
#include <vector>

namespace engine::parse::xml {

[[nodiscard]]
auto parse_model(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<render::model, parse_err>
try {
    using namespace brief_int;
    using namespace brief_int::literals;

    auto const* const model_filename_attr = TRY_NULLABLE_OR(
        node->first_attribute("file"),
        return cpp::failure{parse_err::no_model_filename};
    );

    char const* const model_filename = TRY_NULLABLE_OR(
        model_filename_attr->value(),
        return cpp::failure{parse_err::no_model_filename};
    );

    auto model_file = std::ifstream{model_filename};
    if (not model_file) {
        return cpp::failure{parse_err::no_model_file};
    }

    usize num_vertices;
    model_file >> num_vertices;
    if (model_file.bad()) {
        return cpp::failure{parse_err::io_err};
    }
    if (model_file.fail()) {
        return cpp::failure{parse_err::malformed_num};
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
        return cpp::failure{parse_err::io_err};
    }
    if (model_file.eof()) {
        return render::model {
            .coords = std::move(coords)
        };
    }
    if (model_file.fail()) {
        return cpp::failure{parse_err::malformed_num};
    }
    __builtin_unreachable();
} catch (std::bad_alloc const&) {
    return cpp::failure{parse_err::no_mem};
} catch (std::length_error const&) {
    return cpp::failure{parse_err::no_mem};
}

} // namespace engine::parse::xml
