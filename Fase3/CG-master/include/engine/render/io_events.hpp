#pragma once

namespace engine::render {

auto key_down(unsigned char key, int x, int y) noexcept -> void;

auto key_up(unsigned char key, int x, int y) noexcept -> void;

} // namespace engine::render
