#pragma once

#include <bitset>
#include <limits>

namespace engine::render {

class Keyboard {
  private:
    std::bitset<std::numeric_limits<unsigned char>::max()> keys;

  public:
    [[nodiscard]]
    auto pressed(unsigned char key) const noexcept -> bool;

    auto press(unsigned char key) noexcept -> void;

    auto release(unsigned char key) noexcept -> void;
};

} // namespace engine::render
