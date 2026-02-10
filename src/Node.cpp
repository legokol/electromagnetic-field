#include "Node.h"

void CalcNode::setE(const vec3d& E_) { E = E_; }

void CalcNode::setB(const vec3d& B_) { B = B_; }

void CalcNode::setPhi(double p) { phi = p; }

void CalcNode::setGrad(const vec3d& g) { grad = g; }

vec3d CalcNode::getLoc() const { return loc; }

vec3d CalcNode::getE() const { return E; }

vec3d CalcNode::getB() const { return B; }

double CalcNode::getPhi() const { return phi; }

vec3d CalcNode::getGrad() const { return grad; }
