#include <iostream>
#include "CalcMesh.h"

int main() {
    int n = 100;
    double h = 10;
    ConductorElement charge(vector3D((n - 1) * h / 2, (n - 1) * h / 2, (n - 1) * h / 2), vector3D(), 100);
    vector<ConductorElement> dipole = {
            ConductorElement(vector3D((n - 1) * h / 2, (n - 1) * h / 2, (n - 1) * h / 2), vector3D(), 100),
            ConductorElement(vector3D((n + 1) * h / 2, (n - 1) * h / 2, (n - 1) * h / 2), vector3D(), -100)};
    CalcMesh mesh(n, h);
    //mesh.calculate(charge);
    mesh.calculate(dipole);
    mesh.calculateGrad();
    mesh.snapshot();
    return 0;
}