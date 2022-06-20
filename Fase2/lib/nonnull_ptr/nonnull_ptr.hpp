#pragma once

#include <cassert>
#include <compare>
#include <cstddef>
#include <memory>
#include <type_traits>

namespace ptr {

template <typename T>
    requires (not std::is_reference_v<T>)
class nonnull_ptr {
  private:
    T* ptr;

  public:
    nonnull_ptr() = delete;

    nonnull_ptr(std::nullptr_t) = delete;

    constexpr nonnull_ptr(T* const ptr) noexcept
        : ptr{[ptr] {
            assert(ptr != nullptr);
            return ptr;
        }()}
    {}

    nonnull_ptr(nonnull_ptr const&) = default;

    nonnull_ptr(nonnull_ptr&&) = default;

    auto operator=(nonnull_ptr const&) -> nonnull_ptr& = default;

    auto operator=(nonnull_ptr&&) -> nonnull_ptr& = default;

    ~nonnull_ptr() = default;

    [[nodiscard]]
    auto friend operator<=>(nonnull_ptr, nonnull_ptr) = default;

    [[nodiscard]]
    constexpr operator T*() noexcept {
        return this->ptr;
    }

    [[nodiscard]]
    constexpr operator T&() noexcept {
        return *this->ptr;
    }

    [[nodiscard]]
    auto constexpr operator->() noexcept -> T* {
        return this->ptr;
    }
};

template <typename T>
[[nodiscard]]
auto nonnull_ptr_to(T& val) noexcept -> nonnull_ptr<T> {
    return nonnull_ptr{std::addressof(val)};
}

} // namespace ptr
