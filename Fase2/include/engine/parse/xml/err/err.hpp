#pragma once

#include <brief_int.hpp>

namespace engine::parse::xml {

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
    ambiguous_model_ext,
    no_model_file,
    obj_loader_err,
};

} // namespace engine::parse::xml
