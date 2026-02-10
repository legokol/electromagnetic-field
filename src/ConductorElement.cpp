#include "ConductorElement.h"

void ConductorElement::setI(const vec3d& I_) { I = I_; }

void ConductorElement::setQ(double q_) { q = q_; }

vec3d ConductorElement::getLoc() const { return loc; }

vec3d ConductorElement::getI() const { return I; }

double ConductorElement::getQ() const { return q; }

vec3d ConductorElement::calculateE(const vec3d& r) const {
    double l = r.norm();
    return (9e9 * q / (l * l * l)) * r;
}

vec3d ConductorElement::calculateB(const vec3d& r) const {
    double l = r.norm();
    return (1e-7 / (l * l * l)) * cross(I, r);
}

double ConductorElement::calculatePhi(const vec3d& r) const {
    double l = r.norm();
    return 9e9 * q / l;
}
