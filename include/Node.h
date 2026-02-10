#pragma once

#include "vec3.h"

class CalcNode {
public:
    // Конструктор без всего
    CalcNode() {}

    // Конструктор только с локацией
    CalcNode(const vec3d& loc_) : loc(loc_), E(), B(), phi(0), grad() {}

    // Конструктор с заданием всех параметров (а вдруг)
    CalcNode(const vec3d& loc_, const vec3d& E_, const vec3d& B_, double p)
        : loc(loc_)
        , E(E_)
        , B(B_)
        , phi(p)
        , grad() {}

    void setE(const vec3d& E_);

    void setB(const vec3d& B_);

    void setPhi(double p);

    void setGrad(const vec3d& g);

    vec3d getLoc() const;

    vec3d getE() const;

    vec3d getB() const;

    double getPhi() const;

    vec3d getGrad() const;

private:
    vec3d  loc;
    vec3d  E;     // Electric field
    vec3d  B;     // Magnetic field
    double phi;   // Electric potential
    vec3d  grad;  // Potential gradient
};
