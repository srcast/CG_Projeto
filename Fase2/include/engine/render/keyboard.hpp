#pragma once

#include <bitset>
#include <brief_int.hpp>
#include <limits>

namespace engine::render {

class keyboard {
  private:
    std::bitset<std::numeric_limits<unsigned char>::max()> keys;

  public:
    constexpr keyboard() : keys{} {}

    [[nodiscard]]
    auto constexpr pressed(unsigned char const key) const noexcept -> bool {
        return this->keys[static_cast<brief_int::usize>(key)];
    }

    auto press(unsigned char const key) noexcept -> void {
        this->keys[static_cast<brief_int::usize>(key)] = true;
    }

    auto release(unsigned char const key) noexcept -> void {
        this->keys[static_cast<brief_int::usize>(key)] = false;
    }
};

} // namespace engine::render
