#include "engine/render/keyboard.hpp"

#include <brief_int.hpp>

namespace engine::render {

using namespace brief_int;

[[nodiscard]]
auto Keyboard::pressed(unsigned char const key) const noexcept -> bool {
    return this->keys[static_cast<brief_int::usize>(key)];
}

auto Keyboard::press(unsigned char const key) noexcept -> void {
    this->keys[static_cast<brief_int::usize>(key)] = true;
}

auto Keyboard::release(unsigned char const key) noexcept -> void {
    this->keys[static_cast<brief_int::usize>(key)] = false;
}

} // namespace engine::render
