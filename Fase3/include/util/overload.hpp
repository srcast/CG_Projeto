#pragma once

namespace util {

template <class... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};

} // namespace util
