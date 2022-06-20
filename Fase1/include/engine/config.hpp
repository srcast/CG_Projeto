#pragma once

#include "engine/render/world/world.hpp"

#include <glm/vec4.hpp>
#include <string_view>

namespace engine {
    namespace config {
        auto constexpr prog_name = std::string_view{"engine"};
    } // namespace config

    namespace render::config {
        auto constexpr WIN_TITLE = "engine";

        enum : int {
            DEFAULT_WIN_POS_X = 100,
            DEFAULT_WIN_POS_Y = 100,

            DEFAULT_WIN_WIDTH  = 800,
            DEFAULT_WIN_HEIGHT = 800,
        };
        static_assert(
            DEFAULT_WIN_HEIGHT > 0,
            "DEFAULT_WIN_HEIGHT must be greater than zero to prevent divide by"
                " zero when calculating aspect ratio."
        );

        double constexpr DEFAULT_ASPECT_RATIO
            = static_cast<double>(DEFAULT_WIN_WIDTH)
            / static_cast<double>(DEFAULT_WIN_WIDTH);

        auto constexpr DEFAULT_BG_COLOR = glm::vec4 {
            0.08f, // R
            0.08f, // G
            0.08f, // B
            1.f,   // A
        };

        auto const DEFAULT_WORLD = world {
            .camera = {
                .pos = {9., 3., 30.},
                .lookat = {0., 0., 0.},
                .up = {0., 1., 0.},
                .projection = {90., 0.5, 1000.}
            },
            .root = {},
        };

        enum camera_keys : unsigned char {
            KEY_MOVE_UP = 'w',
            KEY_MOVE_LEFT = 'a',
            KEY_MOVE_DOWN = 's',
            KEY_MOVE_RIGHT = 'd',
        };

        enum : unsigned int { RENDER_TICK_MILLIS = 16 }; // 60 FPS

        double constexpr CAM_TRANSL_FACTOR = 0.06;

    } // namespace render::config
} // namespace engine
