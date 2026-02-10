#include <iostream>
#include "Grid.h"

int main() {
    int                           n      = 200;
    double                        h      = 5;
    std::vector<ConductorElement> dipole = {
        ConductorElement(
            vec3d((n - 1) * h / 2, (n - 1) * h / 2, (n - 1) * h / 2),
            vec3d(),
            100),
        ConductorElement(
            vec3d((n + 1) * h / 2, (n - 1) * h / 2, (n - 1) * h / 2),
            vec3d(),
            -100)};
    CalcMesh mesh(n, h);
    std::cout << "Initialization finished, starting calculation" << std::endl;
    mesh.calculate(dipole);
    mesh.calculateGrad();
    std::cout << "Calculation finished, starting saving" << std::endl;
    mesh.snapshot("dipole.vts");
    std::cout << "Saving finished" << std::endl;
    return 0;
}
