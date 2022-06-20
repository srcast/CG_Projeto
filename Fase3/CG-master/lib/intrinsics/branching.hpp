#pragma once

#include <concepts>
#include <cstdlib>
#include <type_traits>
#include <utility>

namespace intrinsics {

template <std::convertible_to<bool> Expr>
auto constexpr likely(Expr&& expr)
    noexcept(std::is_nothrow_convertible_v<Expr, bool>)
-> bool
{
    return __builtin_expect(static_cast<bool>(std::forward<Expr>(expr)), 1l);
}

template <std::convertible_to<bool> Expr>
auto constexpr unlikely(Expr&& expr)
    noexcept(std::is_nothrow_convertible_v<Expr, bool>)
-> bool
{
    return __builtin_expect(static_cast<bool>(std::forward<Expr>(expr)), 0l);
}

[[noreturn]]
auto inline unreachable() noexcept -> void {
#   ifdef NDEBUG
    __builtin_unreachable();
#   else
    std::abort();
#   endif // NDEBUG
}

template <std::convertible_to<bool> Expr>
auto inline assume(Expr&& expr)
    noexcept(std::is_nothrow_convertible_v<Expr, bool>)
-> void
{
    if (not static_cast<bool>(std::forward<Expr>(expr))) {
        unreachable();
    }
}

} // namespace intrinsics
