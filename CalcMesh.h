#ifndef ELECTROMAGNETIC_FIELD_CALCMESH_H
#define ELECTROMAGNETIC_FIELD_CALCMESH_H

#include "CalcNode.h"
#include <vector>

using std::vector;

class CalcMesh {
public:
    // Кубическая сека
    CalcMesh(unsigned int size, double h);

    // Сетка в виде произвольного параллелепипеда sizeX x sizeY x sizeZ, шаг везде h
    CalcMesh(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, double h);

private:
    vector<vector<vector<CalcNode>>> nodes;
};


#endif //ELECTROMAGNETIC_FIELD_CALCMESH_H
