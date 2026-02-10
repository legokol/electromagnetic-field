#pragma once

#include "vec3.h"

struct ConductorElement {
    vec3d  loc;
    vec3d  I;  // Произведение силы тока на вектор направления
    double q;  // Заряд

    vec3d  E(const vec3d& r) const;
    vec3d  B(const vec3d& r) const;
    double phi(const vec3d& r) const;
};
