#ifndef ELECTROMAGNETIC_FIELD_CONDUCTORELEMENT_H
#define ELECTROMAGNETIC_FIELD_CONDUCTORELEMENT_H

#include "vector3D.h"

class ConductorElement {
public:
    ConductorElement(const vector3D &loc_) : loc(loc_), I(), q(0) {}

    ConductorElement(const vector3D &loc_, const vector3D &I_, double q_) : loc(loc_), I(I_), q(q_) {}

    void setI(const vector3D &I_);

    void setQ(double q_);

    vector3D getLoc() const;

    vector3D getI() const;

    double getQ() const;

    vector3D calculateE(const vector3D &r) const;

    vector3D calculateB(const vector3D &r) const;

    double calculatePhi(const vector3D &r) const;

private:
    vector3D loc;
    vector3D I; // Произведение силы тока на вектор направления
    double q; // Заряд
};


#endif //ELECTROMAGNETIC_FIELD_CONDUCTORELEMENT_H
