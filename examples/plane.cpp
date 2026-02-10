#include <iostream>
#include "Grid.h"

int main() {
    const std::size_t             n = 100;
    const double                  h = 5;
    std::vector<ConductorElement> plane((n / 2) * (n / 2));
    for (std::size_t i = 0; i < n / 2; ++i) {
        for (std::size_t j = 0; j < n / 2; ++j) {
            plane[i * (n / 2) + j] =
                ConductorElement(vec3d{.x = static_cast<double>(n / 4 + i) * h,
                                       .y = static_cast<double>(n / 4 + j) * h,
                                       .z = (n - 1) * h / 2},
                                 vec3d{1, 0, 0},
                                 1);
        }
    }
    Grid mesh(n, h);
    std::cout << "Initialization finished, starting calculation" << std::endl;
    mesh.calculate(plane);
    mesh.calculateGrad();
    std::cout << "Calculation finished, starting saving" << std::endl;
    mesh.snapshot("plane.vts");
    std::cout << "Saving finished" << std::endl;
    return 0;
}
