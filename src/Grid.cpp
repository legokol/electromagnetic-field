#include "Grid.h"

#include <vtkAppendFilter.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLStructuredGridWriter.h>
#include <vtkXMLUnstructuredGridWriter.h>

Grid::Grid(std::size_t nx, std::size_t ny, std::size_t nz, double h)
    : nx_{nx}
    , ny_{ny}
    , nz_{nz}
    , h_{h} {
    for (std::size_t i = 0; i < nx_; ++i) {
        for (std::size_t j = 0; j < ny_; ++j) {
            for (std::size_t k = 0; k < nz_; ++k) {
                at(i, j, k).loc = {static_cast<double>(i) * h,
                                   static_cast<double>(j) * h,
                                   static_cast<double>(k) * h};
            }
        }
    }
}

Grid::Grid(std::size_t size, double h) : Grid{size, size, size, h} {}

void Grid::calculate(const ConductorElement& element) {
    for (auto& [r, E, B, phi, grad] : nodes_) {
        E   = element.E(r);
        B   = element.B(r);
        phi = element.phi(r);
    }
#if 0
    cout << "Nodes calculated: "
         << (i + 1) * nodes[0].size() * nodes[0][0].size() << " out of "
         << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;
#endif
}

void Grid::calculate(const std::vector<ConductorElement>& conductor) {
    for (auto& [r, E, B, phi, grad] : nodes_) {
        for (const auto& element : conductor) {
            E   = E + element.E(r);
            B   = B + element.B(r);
            phi = phi + element.phi(r);
        }
    }

#if 0
           еcout << "Nodes calculated: "
                 << (i + 1) * nodes[0].size() * nodes[0][0].size() << " out of "
                 << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;
#endif
}

void Grid::calculateGrad() {
    const auto grad = [this](std::size_t i, std::size_t j, std::size_t k) {
        return at(i, j, k).grad;
    };
    const auto phi = [this](std::size_t i, std::size_t j, std::size_t k) {
        return at(i, j, k).phi;
    };
    for (std::size_t i = 1; i + 1 < nx_; ++i) {
        for (std::size_t j = 1; j + 1 < ny_; ++j) {
            for (std::size_t k = 1; k + 1 < nz_; ++k) {
                grad(i, j, k) = {
                    .x = (phi(i + 1, j, k) - phi(i - 1, j, k)) / (2 * h_),
                    .y = (phi(i, j + 1, k) - phi(i, j - 1, k)) / (2 * h_),
                    .z = (phi(i, j, k + 1) - phi(i, j, k - 1)) / (2 * h_)};
            }
        }
    }
}

namespace {
[[nodiscard]] vtkSmartPointer<vtkStructuredGrid> getStructuredGrid(
    std::size_t              nx,
    std::size_t              ny,
    std::size_t              nz,
    const std::vector<Node>& nodes) {
    // Сетка в терминах VTK
    vtkSmartPointer<vtkStructuredGrid> structuredGrid =
        vtkSmartPointer<vtkStructuredGrid>::New();
    // Точки сетки в терминах VTK
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Потенциал
    auto phi = vtkSmartPointer<vtkDoubleArray>::New();
    phi->SetName("phi");
    phi->SetNumberOfValues(static_cast<vtkIdType>(nodes.size()));

    // Напряжённость электростатического поля
    auto E = vtkSmartPointer<vtkDoubleArray>::New();
    E->SetName("E");
    E->SetNumberOfComponents(3);
    E->SetNumberOfTuples(static_cast<vtkIdType>(nodes.size()));

    // Индукция магнитного поля
    auto B = vtkSmartPointer<vtkDoubleArray>::New();
    B->SetName("B");
    B->SetNumberOfComponents(3);
    B->SetNumberOfTuples(static_cast<vtkIdType>(nodes.size()));

    // Градиент потенциала
    auto grad = vtkSmartPointer<vtkDoubleArray>::New();
    grad->SetName("grad");
    grad->SetNumberOfComponents(3);
    grad->SetNumberOfTuples(static_cast<vtkIdType>(nodes.size()));

    // Разница градиента и электростатического поля
    auto diff = vtkSmartPointer<vtkDoubleArray>::New();
    diff->SetName("diff");
    diff->SetNumberOfComponents(3);
    diff->SetNumberOfTuples(static_cast<vtkIdType>(nodes.size()));

    for (std::size_t i = 0; i < nodes.size(); ++i) {
        const auto& [r, vE, vB, vPhi, vGrad] = nodes[i];
        const auto  idx                      = static_cast<vtkIdType>(i);
        const vec3d vDiff                    = vE + vGrad;
        points->SetPoint(idx, r.x, r.y, r.z);

        double Et[3] = {vE.x, vE.y, vE.z};
        E->SetTuple(idx, Et);

        double Bt[3] = {vB.x, vB.y, vB.z};
        B->SetTuple(idx, Bt);

        phi->SetValue(idx, vPhi);

        double gradT[3] = {-vGrad.x, -vGrad.y, -vGrad.z};
        grad->SetTuple(idx, gradT);

        double diffT[3] = {vDiff.x, vDiff.y, vDiff.z};
        diff->SetTuple(idx, diffT);
    }

#if 0
                cout << "Nodes saved: "
                     << (i + 1) * nodes[0].size() * nodes[0][0].size() << " out of "
                     << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;
#endif

    // Размер и точки сетки
    structuredGrid->SetDimensions(static_cast<int>(nx),
                                  static_cast<int>(ny),
                                  static_cast<int>(nz));
    structuredGrid->SetPoints(points);

    // Прикрепление данных
    structuredGrid->GetPointData()->AddArray(E);
    structuredGrid->GetPointData()->AddArray(B);
    structuredGrid->GetPointData()->AddArray(phi);
    structuredGrid->GetPointData()->AddArray(grad);
    structuredGrid->GetPointData()->AddArray(diff);
    return structuredGrid;
}
}  // namespace

void Grid::snapshot(const std::filesystem::path& path) const {
    auto structuredGrid = getStructuredGrid(nx_, ny_, nz_, nodes_);

    // Записываем
    vtkSmartPointer<vtkXMLStructuredGridWriter> writer =
        vtkSmartPointer<vtkXMLStructuredGridWriter>::New();
    const std::string fileName = path.string();
    writer->SetFileName(fileName.c_str());
    writer->SetInputData(structuredGrid);
    writer->Write();
}

void Grid::snapshot(const std::filesystem::path&         path,
                    const std::vector<ConductorElement>& conductor) const {
    // Проводник
    vtkSmartPointer<vtkUnstructuredGrid> cGrid =
        vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> cPoints = vtkSmartPointer<vtkPoints>::New();

    // Ток
    auto cI = vtkSmartPointer<vtkDoubleArray>::New();
    cI->SetName("I");
    cI->SetNumberOfComponents(3);
    cI->SetNumberOfTuples(static_cast<vtkIdType>(conductor.size()));

    auto cE = vtkSmartPointer<vtkDoubleArray>::New();
    cE->SetName("E");
    cE->SetNumberOfComponents(3);
    cE->SetNumberOfTuples(static_cast<vtkIdType>(conductor.size()));
    cE->Fill(0);

    auto cB = vtkSmartPointer<vtkDoubleArray>::New();
    cB->SetName("B");
    cB->SetNumberOfComponents(3);
    cB->SetNumberOfTuples(static_cast<vtkIdType>(conductor.size()));
    cB->Fill(0);

    for (std::size_t i = 0; i < conductor.size(); ++i) {
        const vec3d r   = conductor[i].loc;
        const vec3d I   = conductor[i].I;
        const auto  idx = static_cast<vtkIdType>(i);
        cPoints->SetPoint(idx, r.x, r.y, r.z);
        double It[3] = {I.x, I.y, I.z};
        cI->SetTuple(idx, It);
    }

    // Создание сетки для проводника
    cGrid->SetPoints(cPoints);
    cGrid->GetPointData()->AddArray(cI);
    cGrid->GetPointData()->AddArray(cE);
    cGrid->GetPointData()->AddArray(cB);

    auto grid  = getStructuredGrid(nx_, ny_, nz_, nodes_);

    auto gridI = vtkSmartPointer<vtkDoubleArray>::New();
    gridI->SetName("I");
    gridI->SetNumberOfComponents(3);
    gridI->SetNumberOfTuples(static_cast<vtkIdType>(nodes_.size()));
    gridI->Fill(0);
    grid->GetPointData()->AddArray(gridI);

#if 0
            cout << "Nodes saved: "
                 << (i + 1) * nodes[0].size() * nodes[0][0].size() << " out of "
                 << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;
#endif

    vtkSmartPointer<vtkAppendFilter> append =
        vtkSmartPointer<vtkAppendFilter>::New();
    append->AddInputData(grid);
    append->AddInputData(cGrid);
    append->Update();

    // Записываем
    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
        vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    const std::string fileName = path.string();
    writer->SetFileName(fileName.c_str());
    writer->SetInputData(append->GetOutput());
    writer->Write();
}
