#include "ConductorElement.h"

#include <cmath>

inline constexpr double k    = 9e9;
inline constexpr double mupi = 1e-7;

vec3d ConductorElement::E(vec3d r) const {
    r               = r - loc;
    const double l2 = r.squaredNorm();
    if (l2 > 0) {
        return (k * q / (l2 * std::sqrt(l2))) * r;
    }
    return {0, 0, 0};
}

vec3d ConductorElement::B(vec3d r) const {
    r               = r - loc;
    const double l2 = r.squaredNorm();
    if (l2 > 0) {
        return (mupi / (l2 * std::sqrt(l2))) * cross(I, r);
    }
    return {0, 0, 0};
}

double ConductorElement::phi(vec3d r) const {
    r               = r - loc;
    const double l2 = r.squaredNorm();
    if (l2 > 0) {
        return k * q / std::sqrt(l2);
    }
    return 0;
}
