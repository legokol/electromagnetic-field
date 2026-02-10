#pragma once

#include "vec3.h"

struct Node {
    vec3d  loc;
    vec3d  E;     // Electric field
    vec3d  B;     // Magnetic field
    double phi;   // Electric potential
    vec3d  grad;  // Potential gradient
};
