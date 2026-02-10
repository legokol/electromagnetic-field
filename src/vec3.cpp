#include "vec3.h"

#include <cmath>
#include <numbers>

namespace {
constexpr double deg2rad(double x) { return x * std::numbers::pi / 180; }
}  // namespace

double vec3d::norm() const { return std::sqrt(squaredNorm()); }

vec3d rotateX(const vec3d& v, double angle) noexcept {
    angle          = deg2rad(angle);
    const double s = std::sin(angle);
    const double c = std::cos(angle);
    return {.x = v.x, .y = v.y * c + v.z * s, .z = -v.y * s + v.z * c};
}
// Поворачивает вектор вокруг оси Y (единичный вектор (0; 1; 0)) на угол angle, заданный в градусах
vec3d rotateY(const vec3d& v, double angle) noexcept {
    angle          = deg2rad(angle);
    const double s = std::sin(angle);
    const double c = std::cos(angle);
    return {.x = -v.z * s + v.x * c, .y = v.y, .z = v.z * c + v.x * s};
}
// Поворачивает вектор вокруг оси Z (единичный вектор (0; 0; 1)) на угол angle, заданный в градусах
vec3d rotateZ(const vec3d& v, double angle) noexcept {
    angle          = deg2rad(angle);
    const double s = std::sin(angle);
    const double c = std::cos(angle);
    return {.x = v.x * c + v.y * s, .y = -v.x * s + v.y * c, .z = v.z};
}
