#include "CalcNode.h"

void CalcNode::setE(const vector3D &E_) {
    E = E_;
}

void CalcNode::setB(const vector3D &B_) {
    B = B_;
}

void CalcNode::setPhi(double p) {
    phi = p;
}

vector3D CalcNode::getLoc() const {
    return loc;
}

vector3D CalcNode::getE() const {
    return E;
}

vector3D CalcNode::getB() const {
    return B;
}

double CalcNode::getPhi() const {
    return phi;
}
