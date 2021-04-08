#ifndef ELECTROMAGNETIC_FIELD_CALCMESH_H
#define ELECTROMAGNETIC_FIELD_CALCMESH_H

#include "CalcNode.h"
#include "ConductorElement.h"
#include <vector>

#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkXMLStructuredGridWriter.h>
#include <vtkStructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkAppendFilter.h>

using std::vector;

class CalcMesh {
public:
    // Кубическая сека
    CalcMesh(unsigned int size, double h);

    // Сетка в виде произвольного параллелепипеда sizeX x sizeY x sizeZ, шаг везде h
    CalcMesh(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, double h);

    // Считаем по заряду
    void calculate(const ConductorElement &element);

    // Считаем по система зарядов
    void calculate(const vector<ConductorElement> &conductor);

    // Рассчитываем градиент потенциала, когда он уже задан
    void calculateGrad();

    // Сохраняем для просмотра
    void snapshot(std::string name) const;

    // Сохраняем вместе с объектом
    void snapshot(std::string name, const vector<ConductorElement> &conductor) const;

private:
    vector<vector<vector<CalcNode>>> nodes;
    double h;
};


#endif //ELECTROMAGNETIC_FIELD_CALCMESH_H
