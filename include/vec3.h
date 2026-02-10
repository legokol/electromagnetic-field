#pragma once

struct vec3d {
    double x;
    double y;
    double z;

    [[nodiscard]] constexpr double squaredNorm() const noexcept {
        return x * x + y * y + z * z;
    }
    [[nodiscard]] double norm() const;

    [[nodiscard]] constexpr vec3d operator+(const vec3d& v2) const noexcept {
        return {x + v2.x, y + v2.y, z + v2.z};
    }
    [[nodiscard]] constexpr vec3d operator-(const vec3d& v2) const noexcept {
        return {x - v2.x, y - v2.y, z - v2.z};
    }

    [[nodiscard]] constexpr vec3d operator*(double a) const noexcept {
        return {x * a, y * a, z * a};
    }

private:
};

[[nodiscard]] constexpr vec3d operator*(double a, const vec3d& v) noexcept {
    return v * a;
}

[[nodiscard]] constexpr double dot(const vec3d& v1, const vec3d& v2) noexcept {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

[[nodiscard]] constexpr vec3d cross(const vec3d& v1, const vec3d& v2) noexcept {
    return {.x = v1.y * v2.z - v2.y * v1.z,
            .y = v1.z * v2.x - v2.z * v1.x,
            .z = v1.x * v2.y - v2.x * v1.y};
}

// Поворачивает вектор вокруг оси X (единичный вектор (1; 0; 0)) на угол angle, заданный в градусах
[[nodiscard]] vec3d rotateX(const vec3d& v, double angle) noexcept;
// Поворачивает вектор вокруг оси Y (единичный вектор (0; 1; 0)) на угол angle, заданный в градусах
[[nodiscard]] vec3d rotateY(const vec3d& v, double angle) noexcept;
// Поворачивает вектор вокруг оси Z (единичный вектор (0; 0; 1)) на угол angle, заданный в градусах
[[nodiscard]] vec3d rotateZ(const vec3d& v, double angle) noexcept;
