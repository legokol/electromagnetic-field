#include <iostream>
#include "Grid.h"

int main() {
    int                      n = 100;
    double                   h = 5;
    vector<ConductorElement> solenoid;
    for (int j = 0; j < 11; ++j) {
        for (int i = 0; i < 8; ++i) {
            vector3D loc((n - 1) * h / 2 + cos(i * M_PI / 4) * h / 2,
                         (n - 1) * h / 2 + sin(i * M_PI / 4) * h / 2,
                         (n - 1) * h / 2 + (j - 5) * h);
            vector3D I(-sin(i * M_PI / 4), cos(i * M_PI / 4), 0);
            solenoid.push_back(ConductorElement(loc, I, 12.5));
        }
    }
    CalcMesh mesh(n, h);
    cout << "Initialization finished, starting calculation" << endl;
    mesh.calculate(solenoid);
    mesh.calculateGrad();
    cout << "Calculation finished, starting saving" << endl;
    mesh.snapshot("solenoid.vts");
    cout << "Saving finished" << endl;
    return 0;
}
