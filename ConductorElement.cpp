#include "ConductorElement.h"

void ConductorElement::setJ(const vector3D &j_) {
    j = j_;
}

void ConductorElement::setQ(double q_) {
    q = q_;
}

vector3D ConductorElement::getLoc() const {
    return loc;
}

vector3D ConductorElement::getJ() const {
    return j;
}

double ConductorElement::getQ() const {
    return q;
}

vector3D ConductorElement::calculateE(const vector3D &r) const {
    double l = r.magnitude();
    return (q / (l * l * l)) * r;
}

vector3D ConductorElement::calculateB(const vector3D &r) const {
    return vector3D();
}

double ConductorElement::calculatePhi(const vector3D &r) const {
    double l = r.magnitude();
    return q / l;
}
