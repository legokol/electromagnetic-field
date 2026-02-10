#include <iostream>
#include "Grid.h"

int main() {
    const std::size_t             n = 100;
    const double                  h = 5;
    std::vector<ConductorElement> solenoid;
    for (int j = 0; j < 11; ++j) {
        for (int i = 0; i < 8; ++i) {
            vec3d loc((n - 1) * h / 2 + cos(i * M_PI / 4) * h / 2,
                      (n - 1) * h / 2 + sin(i * M_PI / 4) * h / 2,
                      (n - 1) * h / 2 + (j - 5) * h);
            vec3d I(-sin(i * M_PI / 4), cos(i * M_PI / 4), 0);
            solenoid.push_back(ConductorElement(loc, I, 12.5));
        }
    }
    CalcMesh mesh(n, h);
    std::cout << "Initialization finished, starting calculation" << std::endl;
    mesh.calculate(solenoid);
    mesh.calculateGrad();
    std::cout << "Calculation finished, starting saving" << std::endl;
    mesh.snapshot("solenoid.vts");
    std::cout << "Saving finished" << std::endl;
    return 0;
}
