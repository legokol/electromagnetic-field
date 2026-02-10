#include <iostream>
#include "Grid.h"

int main() {
    int                      n      = 200;
    double                   h      = 5;
    vector<ConductorElement> dipole = {
        ConductorElement(
            vector3D((n - 1) * h / 2, (n - 1) * h / 2, (n - 1) * h / 2),
            vector3D(),
            100),
        ConductorElement(
            vector3D((n + 1) * h / 2, (n - 1) * h / 2, (n - 1) * h / 2),
            vector3D(),
            -100)};
    CalcMesh mesh(n, h);
    cout << "Initialization finished, starting calculation" << endl;
    mesh.calculate(dipole);
    mesh.calculateGrad();
    cout << "Calculation finished, starting saving" << endl;
    mesh.snapshot("dipole.vts");
    cout << "Saving finished" << endl;
    return 0;
}
