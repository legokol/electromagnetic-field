#include "ConductorElement.h"

void ConductorElement::setI(const vector3D &I_) {
    I = I_;
}

void ConductorElement::setQ(double q_) {
    q = q_;
}

vector3D ConductorElement::getLoc() const {
    return loc;
}

vector3D ConductorElement::getI() const {
    return I;
}

double ConductorElement::getQ() const {
    return q;
}

vector3D ConductorElement::calculateE(const vector3D &r) const {
    double l = r.magnitude();
    return (9e9 * q / (l * l * l)) * r;
}

vector3D ConductorElement::calculateB(const vector3D &r) const {
    double l = r.magnitude();
    return (1e-7 / (l * l * l)) * crossProduct(I, r);
}

double ConductorElement::calculatePhi(const vector3D &r) const {
    double l = r.magnitude();
    return 9e9 * q / l;
}
