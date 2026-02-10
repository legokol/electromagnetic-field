#pragma once

#include <cmath>

class vector3D {
public:
    vector3D() : x(0), y(0), z(0) {}

    vector3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

    vector3D(const vector3D& v) : x(v.getX()), y(v.getY()), z(v.getZ()) {}

    double getX() const;

    double getY() const;

    double getZ() const;

    double magnitude() const;

    void setX(double _x);

    void setY(double _y);

    void setZ(double _z);

    void rotateX(
        double
            angle);  // Поворачивает вектор вокруг оси X (единичный вектор (1; 0; 0)) на угол angle, заданный в градусах

    void rotateY(
        double
            angle);  // Поворачивает веткор вокруг оси Y (единичный вектор (0; 1; 0)) на угол angle, заданный в градусах

    void rotateZ(
        double
            angle);  // Поворачивает веткор вокруг оси Z (единичный вектор (0; 0; 1)) на угол angle, заданный в градусах

    vector3D& operator=(const vector3D& v);

    bool operator==(const vector3D& v2) const;

    bool operator!=(const vector3D& v2) const;

    vector3D operator+(const vector3D& v2) const;

    vector3D operator-(const vector3D& v2) const;

    vector3D operator*(double a) const;

    double operator*(
        const vector3D& v2) const;  // Скалярное произведение векторов

private:
    double x, y, z;
};

vector3D operator*(double a, const vector3D& v);

vector3D crossProduct(const vector3D& v1, const vector3D& v2);
