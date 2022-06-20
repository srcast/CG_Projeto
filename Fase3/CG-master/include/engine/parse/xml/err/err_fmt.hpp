#pragma once

#include "engine/parse/xml/err/err.hpp"

#include <fmt/format.h>
#include <intrinsics/branching.hpp>

template <>
struct fmt::formatter<engine::parse::xml::ParseErr> {
    using ParseErr = ::engine::parse::xml::ParseErr;

    auto constexpr parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto constexpr format(ParseErr const err, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", [err] {
            switch (err) {
                using enum ParseErr;

                case NO_MEM:
                    return "ran out of memory";
                case IO_ERR:
                    return "input/output error";

                case SYNTAX_ERR:
                    return "XML syntax error";
                case MALFORMED_NUM:
                    return "malformed number";

                case NO_WORLD_NODE:
                    return "no world node found";
                case NO_CAMERA_NODE:
                    return "no camera node found";
                case NO_GROUP_NODE:
                    return "no root group node found";

                case NO_CAMERA_POS_NODE:
                    return "no camera position node found";
                case NO_CAMERA_LOOKAT_NODE:
                    return "no camera lookat node found";
                case NO_CAMERA_UP_NODE:
                    return "no camera up node found";
                case NO_CAMERA_PROJ_NODE:
                    return "no camera projection node found";

                case UNKNOWN_GROUP_CHILD_NODE:
                    return "unrecognized group child node";

                case UNKNOWN_TRANSFORM:
                    return "unrecognized transformation";

                case NO_TRANSLATE_ALIGN_ATTR:
                    return "no translate align attribute found";
                case UNKNOWN_TRANSLATE_ALIGN_ATTR_VALUE:
                    return "unrecognized translate align attribute value";
                case TRANSLATE_POINTS_LT_4:
                    return "attempted to create a dynamic translate with less "
                        "than 4 points";

                case NO_MODEL_FILENAME:
                    return "no model filename attribute";
                case AMBIGUOUS_MODEL_EXT:
                    return "model filename extension is ambiguous - must be "
                        "either .3d or .obj";
                case NO_MODEL_FILE:
                    return "model points to nonexistent file";
                case OBJ_LOADER_ERR:
                    return "object loader failed";
                default:
                    intrinsics::unreachable();
            }
        }());
    }
};
