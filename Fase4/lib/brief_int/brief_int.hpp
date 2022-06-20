#ifndef BRIEF_INT_HPP
#define BRIEF_INT_HPP

#include <cstddef>
#include <cstdint>

namespace brief_int {

///////////
// TYPES //
///////////

// Unsigned.
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using uf8 = std::uint_fast8_t;
using uf16 = std::uint_fast16_t;
using uf32 = std::uint_fast32_t;
using uf64 = std::uint_fast64_t;

using ul8 = std::uint_least8_t;
using ul16 = std::uint_least16_t;
using ul32 = std::uint_least32_t;
using ul64 = std::uint_least64_t;

using umax = std::uintmax_t;
using uptr = std::uintptr_t;
using usize = std::size_t;
using byte = std::byte;

// Signed.
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using if8 = std::int_fast8_t;
using if16 = std::int_fast16_t;
using if32 = std::int_fast32_t;
using if64 = std::int_fast64_t;

using il8 = std::int_least8_t;
using il16 = std::int_least16_t;
using il32 = std::int_least32_t;
using il64 = std::int_least64_t;

using imax = std::intmax_t;
using iptr = std::intptr_t;
using ptrdiff = std::ptrdiff_t;

///////////////////////////////////
// USER DEFINED INTEGER LITERALS //
///////////////////////////////////

namespace literals {

// Unsigned.
auto constexpr operator ""_u8(unsigned long long int const n) noexcept -> u8 {
    return static_cast<u8>(n);
}
auto constexpr operator ""_u16(unsigned long long int const n) noexcept -> u16 {
    return static_cast<u16>(n);
}
auto constexpr operator ""_u32(unsigned long long int const n) noexcept -> u32 {
    return static_cast<u32>(n);
}
auto constexpr operator ""_u64(unsigned long long int const n) noexcept -> u64 {
    return static_cast<u64>(n);
}

auto constexpr operator ""_uf8(unsigned long long int const n) noexcept -> uf8 {
    return static_cast<uf8>(n);
}
auto constexpr operator ""_uf16(unsigned long long int const n) noexcept -> uf16 {
    return static_cast<uf16>(n);
}
auto constexpr operator ""_uf32(unsigned long long int const n) noexcept -> uf32 {
    return static_cast<uf32>(n);
}
auto constexpr operator ""_uf64(unsigned long long int const n) noexcept -> uf64 {
    return static_cast<uf64>(n);
}

auto constexpr operator ""_ul8(unsigned long long int const n) noexcept -> ul8 {
    return static_cast<ul8>(n);
}
auto constexpr operator ""_ul16(unsigned long long int const n) noexcept -> ul16 {
    return static_cast<ul16>(n);
}
auto constexpr operator""_ul32(unsigned long long int const n) noexcept -> ul32 {
    return static_cast<ul32>(n);
}
auto constexpr operator ""_ul64(unsigned long long int const n) noexcept -> ul64 {
    return static_cast<ul64>(n);
}

auto constexpr operator ""_umax(unsigned long long int const n) noexcept -> umax {
    return static_cast<umax>(n);
}
auto constexpr operator ""_uptr(unsigned long long int const n) noexcept -> uptr {
    return static_cast<uptr>(n);
}
auto constexpr operator ""_uz(unsigned long long int const n) noexcept -> usize {
    return static_cast<usize>(n);
}
auto constexpr operator ""_byte(unsigned long long int const n) noexcept -> byte {
    return static_cast<byte>(n);
}

// Signed.
auto constexpr operator ""_i8(unsigned long long int const n) noexcept -> i8 {
    return static_cast<i8>(n);
}
auto constexpr operator ""_i16(unsigned long long int const n) noexcept -> i16 {
    return static_cast<i16>(n);
}
auto constexpr operator ""_i32(unsigned long long int const n) noexcept -> i32 {
    return static_cast<i32>(n);
}
auto constexpr operator ""_i64(unsigned long long int const n) noexcept -> i64 {
    return static_cast<i64>(n);
}

auto constexpr operator ""_if8(unsigned long long int const n) noexcept -> if8 {
    return static_cast<if8>(n);
}
auto constexpr operator ""_if16(unsigned long long int const n) noexcept -> if16 {
    return static_cast<if16>(n);
}
auto constexpr operator ""_if32(unsigned long long int const n) noexcept -> if32 {
    return static_cast<if32>(n);
}
auto constexpr operator ""_if64(unsigned long long int const n) noexcept -> if64 {
    return static_cast<if64>(n);
}

auto constexpr operator ""_il8(unsigned long long int const n) noexcept -> il8 {
    return static_cast<il8>(n);
}
auto constexpr operator ""_il16(unsigned long long int const n) noexcept -> il16 {
    return static_cast<il16>(n);
}
auto constexpr operator ""_il32(unsigned long long int const n) noexcept -> il32 {
    return static_cast<il32>(n);
}
auto constexpr operator ""_il64(unsigned long long int const n) noexcept -> il64 {
    return static_cast<il64>(n);
}

auto constexpr operator ""_imax(unsigned long long int const n) noexcept -> imax {
    return static_cast<imax>(n);
}
auto constexpr operator ""_iptr(unsigned long long int const n) noexcept -> iptr {
    return static_cast<iptr>(n);
}
auto constexpr operator ""_ptrdiff(unsigned long long int const n) noexcept -> ptrdiff {
    return static_cast<ptrdiff>(n);
}

} // namespace literals

} // namespace brief_int

#endif // BRIEF_INT_HPP
