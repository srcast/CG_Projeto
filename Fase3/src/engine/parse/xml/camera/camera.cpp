#include "engine/parse/xml/camera/camera.hpp"

#include "engine/parse/xml/camera/projection.hpp"
#include "engine/parse/xml/util/xyz.hpp"
#include "util/try.hpp"

namespace engine::parse::xml {

auto parse_camera(rapidxml::xml_node<> const* const node) noexcept
    -> cpp::result<render::Camera, ParseErr>
{
    auto const* const pos_node = TRY_NULLABLE_OR(
        node->first_node("position"),
        return cpp::fail(ParseErr::NO_CAMERA_POS_NODE)
    );
    auto const* const lookat_node = TRY_NULLABLE_OR(
        node->first_node("lookAt"),
        return cpp::fail(ParseErr::NO_CAMERA_LOOKAT_NODE)
    );
    auto const* const up_node = TRY_NULLABLE_OR(
        node->first_node("up"),
        return cpp::fail(ParseErr::NO_CAMERA_UP_NODE)
    );
    auto const* const proj_node = TRY_NULLABLE_OR(
        node->first_node("projection"),
        return cpp::fail(ParseErr::NO_CAMERA_PROJ_NODE)
    );

    return render::Camera {
        .pos    = TRY_RESULT(util::parse_xyz(pos_node)),
        .lookat = TRY_RESULT(util::parse_xyz(lookat_node)),
        .up     = TRY_RESULT(util::parse_xyz(up_node)),
        .projection = TRY_RESULT(parse_projection(proj_node)),
    };
}

} // namespace engine::parse::xml
