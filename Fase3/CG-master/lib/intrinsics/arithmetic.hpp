#pragma once

#include <concepts>
#include <type_traits>

namespace intrinsics {

template <typename T>
    requires (
        std::integral<T>
        and not (std::same_as<T, bool> or std::is_enum_v<T>)
    )
struct overflow_result {
    bool overflowed;
    T result;
};

template <typename T>
overflow_result(bool, T) -> overflow_result<T>;

template <
    typename T1,
    typename T2,
    typename R = std::common_type_t<T1, T2>
>
auto constexpr add_overflow(T1 op1, T2 op2) noexcept -> overflow_result<R>
    requires (
        (std::integral<T1> or std::is_enum_v<T1>)
        and
        (std::integral<T2> or std::is_enum_v<T2>)
    )
{
    R result;
    auto const overflowed = __builtin_add_overflow(op1, op2, &result);
    return overflow_result{overflowed, result};
}

template <
    typename T1,
    typename T2,
    typename R = std::common_type_t<T1, T2>
>
auto constexpr sub_overflow(T1 op1, T2 op2) noexcept -> overflow_result<R>
    requires (
        (std::integral<T1> or std::is_enum_v<T1>)
        and
        (std::integral<T2> or std::is_enum_v<T2>)
    )
{
    R result;
    auto const overflowed = __builtin_sub_overflow(op1, op2, &result);
    return overflow_result{overflowed, result};
}

template <
    typename T1,
    typename T2,
    typename R = std::common_type_t<T1, T2>
>
auto constexpr mul_overflow(T1 op1, T2 op2) noexcept -> overflow_result<R>
    requires (
        (std::integral<T1> or std::is_enum_v<T1>)
        and
        (std::integral<T2> or std::is_enum_v<T2>)
    )
{
    R result;
    auto const overflowed = __builtin_mul_overflow(op1, op2, &result);
    return overflow_result{overflowed, result};
}

} // namespace intrinsics
