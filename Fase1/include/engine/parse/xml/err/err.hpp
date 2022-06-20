#pragma once

#include <brief_int.hpp>

namespace engine::parse::xml {

// NOTE: changing the order of the enumerators requires a change in the
// string formatting implementation.
enum class parse_err : brief_int::usize {
    no_mem,
    io_err,

    syntax_err,
    malformed_num,

    no_world_node,
    no_camera_node,
    no_group_node,

    no_camera_pos_node,
    no_camera_lookat_node,
    no_camera_up_node,
    no_camera_proj_node,

    unknown_group_child_node,

    unknown_transform,

    no_model_filename,
    no_model_file,
};

} // namespace engine::parse::xml
