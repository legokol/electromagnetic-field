#include "ConductorElement.h"

inline constexpr double k    = 9e9;
inline constexpr double mupi = 1e-7;

vec3d ConductorElement::E(const vec3d& r) const {
    double l = r.norm();
    return (k * q / (l * l * l)) * r;
}

vec3d ConductorElement::B(const vec3d& r) const {
    double l = r.norm();
    return (mupi / (l * l * l)) * cross(I, r);
}

double ConductorElement::phi(const vec3d& r) const {
    double l = r.norm();
    return k * q / l;
}
