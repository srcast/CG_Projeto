#include "util/coord_conv.hpp"

#include <glm/exponential.hpp>
#include <glm/trigonometric.hpp>

namespace util {

[[nodiscard]]
auto cartesian_to_cylindrical(glm::vec3 const& cartesian) noexcept -> glm::vec3 {
    auto const x = cartesian[0];
    auto const y = cartesian[1];
    auto const z = cartesian[2];

    auto const radial_dist = glm::sqrt(x * x + z * z);
    auto const azimuth = (x == 0.f and y == 0.f) ? 0.f : glm::atan(x, z);

    return glm::vec3{radial_dist, y, azimuth};
}

[[nodiscard]]
auto cylindrical_to_cartesian(glm::vec3 const& cylindrical) noexcept -> glm::vec3 {
    auto const radial_dist = cylindrical[0];
    auto const y = cylindrical[1];
    auto const azimuth = cylindrical[2];

    auto const x = radial_dist * glm::sin(azimuth);
    auto const z = radial_dist * glm::cos(azimuth);

    return glm::vec3{x, y, z};
}

[[nodiscard]]
auto cartesian_to_spherical(glm::vec3 const& cartesian) noexcept -> glm::vec3 {
    auto const x = cartesian[0];
    auto const y = cartesian[1];
    auto const z = cartesian[2];

    auto const radius = glm::sqrt(x * x + y * y + z * z);
    float polar;
    float azimuth;
    if (x == 0.f and z == 0.f) {
        if (y == 0.f) {
            // If the point is on the origin, assume an inclination of zero.
            polar = 0.f;
        } else {
            polar = glm::acos(y / radius);
        }
        // If the point is on the y axis, assume an azimuth of zero.
        azimuth = 0.f;
    } else {
        polar = glm::acos(y / radius);
        azimuth = glm::atan(x, z);
    }

    return glm::vec3{radius, polar, azimuth};
}

[[nodiscard]]
auto spherical_to_cartesian(glm::vec3 const& spherical) noexcept -> glm::vec3 {
    auto const radius = spherical[0];
    auto const polar = spherical[1];
    auto const azimuth = spherical[2];

    auto const radius_sin_polar = radius * glm::sin(polar);

    auto const x = radius_sin_polar * glm::sin(azimuth);
    auto const y = radius * glm::cos(polar);
    auto const z = radius_sin_polar * glm::cos(azimuth);

    return glm::vec3{x, y, z};
}

[[nodiscard]]
auto cylindrical_to_spherical(glm::vec3 const& cylindrical) noexcept -> glm::vec3 {
    auto const radial_dist = cylindrical[0];
    auto const y = cylindrical[1];
    auto const azimuth = cylindrical[2];

    auto const radius = glm::sqrt(radial_dist * radial_dist + y * y);
    auto const polar = (radial_dist == 0.f and y == 0.f)
        ? 0.f // Assume zero.
        : glm::atan(radial_dist, y);

    return glm::vec3{radius, polar, azimuth};
}

[[nodiscard]]
auto spherical_to_cylindrical(glm::vec3 const& spherical) noexcept -> glm::vec3 {
    auto const radius = spherical[0];
    auto const polar = spherical[1];
    auto const azimuth = spherical[2];

    auto const radial_dist = radius * glm::sin(polar);
    auto const y = glm::cos(polar);

    return glm::vec3{radial_dist, y, azimuth};
}

auto cartesian_to_cylindrical_inplace(glm::vec3& cartesian) noexcept -> void {
    auto const x = cartesian[0];
    auto const y = cartesian[1];
    auto const z = cartesian[2];

    auto const radial_dist = glm::sqrt(x * x + z * z);
    auto const azimuth = (x == 0.f and y == 0.f) ? 0.f : glm::atan(x, z);

    cartesian[0] = radial_dist;
    cartesian[2] = azimuth;
}

auto cylindrical_to_cartesian_inplace(glm::vec3& cylindrical) noexcept -> void {
    auto const radial_dist = cylindrical[0];
    auto const azimuth = cylindrical[2];

    auto const x = radial_dist * glm::sin(azimuth);
    auto const z = radial_dist * glm::cos(azimuth);

    cylindrical[0] = x;
    cylindrical[2] = z;
}

auto cartesian_to_spherical_inplace(glm::vec3& cartesian) noexcept -> void {
    auto const x = cartesian[0];
    auto const y = cartesian[1];
    auto const z = cartesian[2];

    auto const radius = glm::sqrt(x * x + y * y + z * z);
    float polar;
    float azimuth;
    if (x == 0.f and z == 0.f) {
        if (y == 0.f) {
            // If the point is on the origin, assume an inclination of zero.
            polar = 0.f;
        } else {
            polar = glm::acos(y / radius);
        }
        // If the point is on the y axis, assume an azimuth of zero.
        azimuth = 0.f;
    } else {
        polar = glm::acos(y / radius);
        azimuth = glm::atan(x, z);
    }

    cartesian[0] = radius;
    cartesian[1] = polar;
    cartesian[2] = azimuth;
}

auto spherical_to_cartesian_inplace(glm::vec3& spherical) noexcept -> void {
    auto const radius = spherical[0];
    auto const polar = spherical[1];
    auto const azimuth = spherical[2];

    auto const radius_sin_polar = radius * glm::sin(polar);

    auto const x = radius_sin_polar * glm::sin(azimuth);
    auto const y = radius * glm::cos(polar);
    auto const z = radius_sin_polar * glm::cos(azimuth);

    spherical[0] = x;
    spherical[1] = y;
    spherical[2] = z;
}

auto cylindrical_to_spherical_inplace(glm::vec3& cylindrical) noexcept -> void {
    auto const radial_dist = cylindrical[0];
    auto const y = cylindrical[1];

    auto const radius = glm::sqrt(radial_dist * radial_dist + y * y);
    auto const polar = (radial_dist == 0.f and y == 0.f)
        ? 0.f // Assume zero.
        : glm::atan(radial_dist, y);

    cylindrical[0] = radius;
    cylindrical[1] = polar;
}

auto spherical_to_cylindrical_inplace(glm::vec3& spherical) noexcept -> void {
    auto const radius = spherical[0];
    auto const polar = spherical[1];

    auto const radial_dist = radius * glm::sin(polar);
    auto const y = glm::cos(polar);

    spherical[0] = radial_dist;
    spherical[1] = y;
}

} // namespace util
