#include <iostream>
#include "Grid.h"

int main() {
    int                      n = 100;
    double                   h = 5;
    vector<ConductorElement> plane((n / 2) * (n / 2));
    for (int i = 0; i < n / 2; ++i) {
        for (int j = 0; j < n / 2; ++j) {
            double x = (n / 4 + i) * h;
            double y = (n / 4 + j) * h;
            double z = (n - 1) * h / 2;
            plane[i * (n / 2) + j] =
                ConductorElement(vector3D(x, y, z), vector3D(1, 0, 0), 1);
        }
    }
    CalcMesh mesh(n, h);
    cout << "Initialization finished, starting calculation" << endl;
    mesh.calculate(plane);
    mesh.calculateGrad();
    cout << "Calculation finished, starting saving" << endl;
    mesh.snapshot("plane.vts");
    cout << "Saving finished" << endl;
    return 0;
}
