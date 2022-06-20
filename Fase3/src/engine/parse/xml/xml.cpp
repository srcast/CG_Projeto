#include "engine/parse/xml/xml.hpp"

#include "engine/parse/xml/camera/camera.hpp"
#include "engine/parse/xml/group/group.hpp"
#include "util/try.hpp"

#include <exception>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

namespace {

using engine::parse::xml::ParseErr;

[[nodiscard]]
auto static open_xml_file(char const* const xml_filepath) noexcept
    -> cpp::result<rapidxml::file<>, ParseErr>
{
    try {
        return rapidxml::file{xml_filepath};
    } catch (std::exception const&) {
        return cpp::fail(ParseErr::IO_ERR);
    }
}

} // anonymous namespace

namespace engine::parse::xml {

auto parse_xml(char const* const xml_filepath) noexcept
    -> cpp::result<
        std::pair<render::World, render::Camera>,
        ParseErr
    >
{
    auto input_file = TRY_RESULT(open_xml_file(xml_filepath));
    auto xml_doc = rapidxml::xml_document{};
    try {
        xml_doc.parse<rapidxml::parse_default>(input_file.data());
    } catch (rapidxml::parse_error const&) {
        return cpp::fail(ParseErr::SYNTAX_ERR);
    }

    auto const* const world_node = TRY_NULLABLE_OR(
        xml_doc.first_node("world"),
        return cpp::fail(ParseErr::NO_WORLD_NODE)
    );

    auto const* const camera_node = TRY_NULLABLE_OR(
        world_node->first_node("camera"),
        return cpp::fail(ParseErr::NO_CAMERA_NODE);
    );

    auto const* const group_node = TRY_NULLABLE_OR(
        world_node->first_node("group"),
        return cpp::fail(ParseErr::NO_GROUP_NODE);
    );

    return std::pair {
        render::World {
            .root =  TRY_RESULT(parse_group(group_node))
        },
        render::Camera {
            TRY_RESULT(parse_camera(camera_node))
        },
    };
}

} // namespace engine::parse::xml
