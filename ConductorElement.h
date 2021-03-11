#ifndef ELECTROMAGNETIC_FIELD_CONDUCTORELEMENT_H
#define ELECTROMAGNETIC_FIELD_CONDUCTORELEMENT_H

#include "vector3D.h"

class ConductorElement {
public:
    ConductorElement(const vector3D &loc_) : loc(loc_), j(), q(0) {}

    ConductorElement(const vector3D &loc_, const vector3D &j_, double q_) : loc(loc_), j(j_), q(q_) {}

    void setJ(const vector3D &j_);

    void setQ(double q_);

    vector3D getLoc() const;

    vector3D getJ() const;

    double getQ() const;

    vector3D calculateE(const vector3D &r) const;

    vector3D calculateB(const vector3D &r) const;

    double caculatePhi(const vector3D &r) const;

private:
    vector3D loc;
    vector3D j; // Плотность тока
    double q; // Заряд
};


#endif //ELECTROMAGNETIC_FIELD_CONDUCTORELEMENT_H
