#pragma once

#include <filesystem>
#include <vector>

#include "ConductorElement.h"
#include "Node.h"

class Grid {
public:
    // Кубическая сетка
    Grid(std::size_t size, double h);

    // Сетка в виде произвольного параллелепипеда sizeX x sizeY x sizeZ, шаг везде h
    Grid(std::size_t nx, std::size_t ny, std::size_t nz, double h);

    // Считаем по заряду
    void calculate(const ConductorElement& element);

    // Считаем по системе зарядов
    void calculate(const std::vector<ConductorElement>& conductor);

    // Рассчитываем градиент потенциала, когда он уже задан
    void calculateGrad();

    // Сохраняем для просмотра
    void snapshot(const std::filesystem::path& path) const;

    // Сохраняем вместе с объектом
    void snapshot(const std::filesystem::path&         path,
                  const std::vector<ConductorElement>& conductor) const;

private:
    std::size_t       nx_;
    std::size_t       ny_;
    std::size_t       nz_;
    double            h_;
    std::vector<Node> nodes_ = std::vector<Node>(nx_ * ny_ * nz_);

    [[nodiscard]] Node& at(std::size_t i,
                           std::size_t j,
                           std::size_t k) noexcept {
        return nodes_[(i * ny_ + j) * nz_ + k];
    }
    [[nodiscard]] const Node& at(std::size_t i,
                                 std::size_t j,
                                 std::size_t k) const noexcept {
        return nodes_[(i * ny_ + j) * nz_ + k];
    }
};
