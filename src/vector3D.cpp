#include "vector3D.h"
#include <cmath>
#include <iostream>

using namespace std;

double vector3D::getX() const { return x; }

double vector3D::getY() const { return y; }

double vector3D::getZ() const { return z; }

double vector3D::magnitude() const { return sqrt(x * x + y * y + z * z); }

void vector3D::setX(double _x) { x = _x; }

void vector3D::setY(double _y) { y = _y; }

void vector3D::setZ(double _z) { z = _z; }

void vector3D::rotateX(double angle) {
    angle     = angle * 3.14 / 180;
    double _y = getY(), _z = getZ();
    setY(_y * cos(angle) + _z * sin(angle));
    setZ(-_y * sin(angle) + _z * cos(angle));
}

void vector3D::rotateY(double angle) {
    angle     = angle * 3.14 / 180;
    double _z = getZ(), _x = getX();
    setZ(_z * cos(angle) + _x * sin(angle));
    setX(-_z * sin(angle) + _x * cos(angle));
}

void vector3D::rotateZ(double angle) {
    angle     = angle * 3.14 / 180;
    double _x = getX(), _y = getY();
    setX(_x * cos(angle) + _y * sin(angle));
    setY(-_x * sin(angle) + _y * cos(angle));
}

vector3D& vector3D::operator=(const vector3D& v) {
    x = v.getX();
    y = v.getY();
    z = v.getZ();
    return (*this);
}

bool vector3D::operator==(const vector3D& v2) const {
    return x == v2.getX() && y == v2.getY();
}

bool vector3D::operator!=(const vector3D& v2) const { return !(*this == v2); }

vector3D vector3D::operator+(const vector3D& v2) const {
    return vector3D(x + v2.getX(), y + v2.getY(), z + v2.getZ());
}

vector3D vector3D::operator-(const vector3D& v2) const {
    return vector3D(x - v2.getX(), y - v2.getY(), z - v2.getZ());
}

vector3D vector3D::operator*(double a) const {
    return vector3D(x * a, y * a, z * a);
}

double vector3D::operator*(const vector3D& v2) const {
    return x * v2.getX() + y * v2.getY() + z * v2.getZ();
}

vector3D operator*(double a, const vector3D& v) { return v * a; }

vector3D crossProduct(const vector3D& v1, const vector3D& v2) {
    double x = v1.getY() * v2.getZ() - v2.getY() * v1.getZ();
    double y = v1.getZ() * v2.getX() - v2.getZ() * v1.getX();
    double z = v1.getX() * v2.getY() - v2.getX() * v1.getY();
    return vector3D(x, y, z);
}
