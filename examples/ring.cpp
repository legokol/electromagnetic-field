#include <iostream>
#include "Grid.h"

int main() {
    const std::size_t             n = 100;
    const double                  h = 5;
    std::vector<ConductorElement> ring;
    for (int i = 0; i < 8; ++i) {
        vec3d loc((n - 1) * h / 2 + cos(i * M_PI / 4) * h / 2,
                  (n - 1) * h / 2 + sin(i * M_PI / 4) * h / 2,
                  (n - 1) * h / 2);
        vec3d I(-sin(i * M_PI / 4), cos(i * M_PI / 4), 0);
        ring.push_back(ConductorElement(loc, I, 12.5));
    }
    Grid mesh(n, h);
    std::cout << "Initialization finished, starting calculation" << std::endl;
    mesh.calculate(ring);
    mesh.calculateGrad();
    std::cout << "Calculation finished, starting saving" << std::endl;
    mesh.snapshot("ring.vtu", ring);
    std::cout << "Saving finished" << std::endl;
    return 0;
}
