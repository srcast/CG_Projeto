#pragma once

namespace engine::render {
    // Forward declarations.
    struct world;
    class renderer;

    auto launch() noexcept -> renderer&;

    class renderer {
      private:
        friend auto launch() noexcept -> renderer&;
        renderer() noexcept;

      public:
        auto set_world(world& world) noexcept -> renderer&;

        [[noreturn]]
        auto run() noexcept -> void;
    };
} // namespace engine::render
