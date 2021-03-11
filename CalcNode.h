#ifndef ELECTROMAGNETIC_FIELD_CALCNODE_H
#define ELECTROMAGNETIC_FIELD_CALCNODE_H

#include "vector3D.h"

class CalcNode {
public:
    // Конструктор только с локацией
    CalcNode(const vector3D &loc_) : loc(loc_), E(), B(), phi(0) {}

    // Конструктор с заданием всех параметров (а вдруг)
    CalcNode(const vector3D &loc_, const vector3D &E_, const vector3D &B_, double p) : loc(loc_), E(E_), B(B_),
                                                                                       phi(p) {}

    void setE(const vector3D &E_);

    void setB(const vector3D &B_);

    void setPhi(double p);

    vector3D getLoc() const;

    vector3D getE() const;

    vector3D getB() const;

    double getPhi() const;

private:
    vector3D loc;
    vector3D E; // Electric field
    vector3D B; // Magnetic field
    double phi; // Electric potential
};


#endif //ELECTROMAGNETIC_FIELD_CALCNODE_H
