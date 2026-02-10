#pragma once

#include <vector>

#include "ConductorElement.h"
#include "Node.h"

class CalcMesh {
public:
    // Кубическая сетка
    CalcMesh(unsigned int size, double h);

    // Сетка в виде произвольного параллелепипеда sizeX x sizeY x sizeZ, шаг везде h
    CalcMesh(unsigned int sizeX,
             unsigned int sizeY,
             unsigned int sizeZ,
             double       h);

    // Считаем по заряду
    void calculate(const ConductorElement& element);

    // Считаем по система зарядов
    void calculate(const std::vector<ConductorElement>& conductor);

    // Рассчитываем градиент потенциала, когда он уже задан
    void calculateGrad();

    // Сохраняем для просмотра
    void snapshot(std::string name) const;

    // Сохраняем вместе с объектом
    void snapshot(std::string                          name,
                  const std::vector<ConductorElement>& conductor) const;

private:
    std::vector<std::vector<std::vector<CalcNode>>> nodes;
    double                                          h;
};
