#pragma once

#include "engine/parse/xml/err/err.hpp"

#include <fmt/format.h>

template <>
struct fmt::formatter<engine::parse::xml::parse_err> {
    using parse_err = ::engine::parse::xml::parse_err;

    auto constexpr parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto constexpr format(parse_err const err, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", [err] {
            switch (err) {
                using enum ::engine::parse::xml::parse_err;

                case no_mem:
                    return "ran out of memory";
                case io_err:
                    return "input/output error";

                case syntax_err:
                    return "XML syntax error";
                case malformed_num:
                    return "malformed number";

                case no_world_node:
                    return "no world node found";
                case no_camera_node:
                    return "no camera node found";
                case no_group_node:
                    return "no root group node found";

                case no_camera_pos_node:
                    return "no camera position node found";
                case no_camera_lookat_node:
                    return "no camera lookat node found";
                case no_camera_up_node:
                    return "no camera up node found";
                case no_camera_proj_node:
                    return "no camera projection node found";

                case unknown_group_child_node:
                    return "unrecognized group child node";

                case unknown_transform:
                    return "unrecognized transformation";

                case no_model_filename:
                    return "no model filename attribute";
                case ambiguous_model_ext:
                    return "model filename extension is ambiguous - must be "
                        "either .3d or .obj";
                case no_model_file:
                    return "model points to nonexistent file";
                case obj_loader_err:
                    return "object loader failed";
                default:
                    __builtin_unreachable();
            }
        }());
    }
};
