#pragma once

#include <concepts>

namespace util {

template <typename T>
concept number = std::integral<T> or std::floating_point<T>;

} // namespace util
