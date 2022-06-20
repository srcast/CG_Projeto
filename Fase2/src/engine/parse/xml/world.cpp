#include "engine/parse/xml/world.hpp"

#include "engine/parse/xml/camera/camera.hpp"
#include "engine/parse/xml/group/group.hpp"
#include "util/try.hpp"

#include <exception>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

namespace {

using engine::parse::xml::parse_err;

[[nodiscard]]
auto open_xml_file(char const* const xml_filepath) noexcept
    -> cpp::result<rapidxml::file<>, parse_err>
{
    try {
        return rapidxml::file{xml_filepath};
    } catch (std::exception const&) {
        return cpp::fail(parse_err::io_err);
    }
}

} // anonymous namespace

namespace engine::parse::xml {

auto parse_world(char const* const xml_filepath) noexcept
    -> cpp::result<render::world, parse_err>
{
    auto input_file = TRY_RESULT(open_xml_file(xml_filepath));
    auto xml_doc = rapidxml::xml_document{};
    try {
        xml_doc.parse<rapidxml::parse_default>(input_file.data());
    } catch (rapidxml::parse_error const&) {
        return cpp::fail(parse_err::syntax_err);
    }

    auto const* const world_node = TRY_NULLABLE_OR(
        xml_doc.first_node("world"),
        return cpp::fail(parse_err::no_world_node)
    );

    auto const* const camera_node = TRY_NULLABLE_OR(
        world_node->first_node("camera"),
        return cpp::fail(parse_err::no_camera_node);
    );

    auto const* const group_node = TRY_NULLABLE_OR(
        world_node->first_node("group"),
        return cpp::fail(parse_err::no_group_node);
    );

    return render::world {
        .camera = TRY_RESULT(parse_camera(camera_node)),
        .root = TRY_RESULT(parse_group(group_node)),
    };
}

} // namespace engine::parse::xml
