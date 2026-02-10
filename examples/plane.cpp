#include <iostream>
#include "Grid.h"

int main() {
    int                           n = 100;
    double                        h = 5;
    std::vector<ConductorElement> plane((n / 2) * (n / 2));
    for (int i = 0; i < n / 2; ++i) {
        for (int j = 0; j < n / 2; ++j) {
            double x = (n / 4 + i) * h;
            double y = (n / 4 + j) * h;
            double z = (n - 1) * h / 2;
            plane[i * (n / 2) + j] =
                ConductorElement(vec3d(x, y, z), vec3d(1, 0, 0), 1);
        }
    }
    CalcMesh mesh(n, h);
    std::cout << "Initialization finished, starting calculation" << std::endl;
    mesh.calculate(plane);
    mesh.calculateGrad();
    std::cout << "Calculation finished, starting saving" << std::endl;
    mesh.snapshot("plane.vts");
    std::cout << "Saving finished" << std::endl;
    return 0;
}
