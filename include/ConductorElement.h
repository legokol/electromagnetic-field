#pragma once

#include "vec3.h"

class ConductorElement {
public:
    ConductorElement() : loc(), I(), q(0) {}

    ConductorElement(const vec3d& loc_) : loc(loc_), I(), q(0) {}

    ConductorElement(const vec3d& loc_, const vec3d& I_, double q_)
        : loc(loc_)
        , I(I_)
        , q(q_) {}

    void setI(const vec3d& I_);

    void setQ(double q_);

    vec3d getLoc() const;

    vec3d getI() const;

    double getQ() const;

    vec3d calculateE(const vec3d& r) const;

    vec3d calculateB(const vec3d& r) const;

    double calculatePhi(const vec3d& r) const;

private:
    vec3d  loc;
    vec3d  I;  // Произведение силы тока на вектор направления
    double q;  // Заряд
};
