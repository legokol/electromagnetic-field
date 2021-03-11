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

const vector3D CalcNode::getLoc() {
    return loc;
}

const vector3D CalcNode::getE() {
    return E;
}

const vector3D CalcNode::getB() {
    return B;
}

const double CalcNode::getPhi() {
    return phi;
}
