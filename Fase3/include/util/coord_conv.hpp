#pragma once

#include <glm/vec3.hpp>

namespace util {

[[nodiscard]]
auto cartesian_to_cylindrical(glm::vec3 const& cartesian) noexcept
    -> glm::vec3;

[[nodiscard]]
auto cylindrical_to_cartesian(glm::vec3 const& cylindrical) noexcept
    -> glm::vec3;

[[nodiscard]]
auto cartesian_to_spherical(glm::vec3 const& cartesian) noexcept
    -> glm::vec3;

[[nodiscard]]
auto spherical_to_cartesian(glm::vec3 const& spherical) noexcept
    -> glm::vec3;

[[nodiscard]]
auto cylindrical_to_spherical(glm::vec3 const& cylindrical) noexcept
    -> glm::vec3;

[[nodiscard]]
auto spherical_to_cylindrical(glm::vec3 const& spherical) noexcept
    -> glm::vec3;

auto cartesian_to_cylindrical_inplace(glm::vec3& cartesian) noexcept -> void;

auto cylindrical_to_cartesian_inplace(glm::vec3& cylindrical) noexcept -> void;

auto cartesian_to_spherical_inplace(glm::vec3& cartesian) noexcept -> void;

auto spherical_to_cartesian_inplace(glm::vec3& spherical) noexcept -> void;

auto cylindrical_to_spherical_inplace(glm::vec3& cylindrical) noexcept -> void;

auto spherical_to_cylindrical_inplace(glm::vec3& spherical) noexcept -> void;

} // namespace util
