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

CalcMesh::CalcMesh(unsigned int size, double h) {
    this->h = h;
    nodes.resize(size);
    for (unsigned int i = 0; i < size; ++i) {
        nodes[i].resize(size);
        for (unsigned int j = 0; j < size; ++j) {
            nodes[i][j].resize(size);
            for (unsigned int k = 0; k < size; ++k) {
                // Позиция
                vec3d loc(i * h, j * h, k * h);
                nodes[i][j][k] = CalcNode(loc);
            }
        }
    }
}

CalcMesh::CalcMesh(unsigned int sizeX,
                   unsigned int sizeY,
                   unsigned int sizeZ,
                   double       h) {
    this->h = h;
    nodes.resize(sizeX);
    for (unsigned int i = 0; i < sizeX; ++i) {
        nodes[i].resize(sizeY);
        for (unsigned int j = 0; j < sizeY; ++j) {
            nodes[i][j].resize(sizeZ);
            for (unsigned int k = 0; k < sizeZ; ++k) {
                // Позиция
                vec3d loc(i * h, j * h, k * h);
                nodes[i][j][k] = CalcNode(loc);
            }
        }
    }
}

void CalcMesh::calculate(const ConductorElement& element) {
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        for (unsigned int j = 0; j < nodes[i].size(); ++j) {
            for (unsigned int k = 0; k < nodes[i][j].size(); ++k) {
                vec3d r = nodes[i][j][k].getLoc() - element.getLoc();
                if (abs(r.x) > 0 || abs(r.y) > 0 || abs(r.z) > 0) {
                    nodes[i][j][k].setE(element.calculateE(r));
                    nodes[i][j][k].setB(element.calculateB(r));
                    nodes[i][j][k].setPhi(element.calculatePhi(r));
                }
            }
            cout << "Nodes calculated: "
                 << (i + 1) * nodes[0].size() * nodes[0][0].size() << " out of "
                 << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;
        }
    }
}

void CalcMesh::calculate(const std::vector<ConductorElement>& conductor) {
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        for (unsigned int j = 0; j < nodes[i].size(); ++j) {
            for (unsigned int k = 0; k < nodes[i][j].size(); ++k) {
                for (unsigned int s = 0; s < conductor.size(); ++s) {
                    vec3d r = nodes[i][j][k].getLoc() - conductor[s].getLoc();
                    if (abs(r.x) > 0 || abs(r.y) > 0 || abs(r.z) > 0) {
                        nodes[i][j][k].setE(nodes[i][j][k].getE()
                                            + conductor[s].calculateE(r));
                        nodes[i][j][k].setB(nodes[i][j][k].getB()
                                            + conductor[s].calculateB(r));
                        nodes[i][j][k].setPhi(nodes[i][j][k].getPhi()
                                              + conductor[s].calculatePhi(r));
                    }
                }
            }
        }
        cout << "Nodes calculated: "
             << (i + 1) * nodes[0].size() * nodes[0][0].size() << " out of "
             << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;
    }
}

void CalcMesh::calculateGrad() {
    for (unsigned int i = 1; i < nodes.size() - 1; ++i) {
        for (unsigned int j = 1; j < nodes[i].size() - 1; ++j) {
            for (unsigned int k = 1; k < nodes[i][j].size() - 1; ++k) {
                nodes[i][j][k].setGrad({.x = (nodes[i + 1][j][k].getPhi()
                                              - nodes[i - 1][j][k].getPhi())
                                           / (2 * h),
                                        .y = (nodes[i][j + 1][k].getPhi()
                                              - nodes[i][j - 1][k].getPhi())
                                           / (2 * h),
                                        .z = (nodes[i][j][k + 1].getPhi()
                                              - nodes[i][j][k - 1].getPhi())
                                           / (2 * h)});
            }
        }
    }
}

void CalcMesh::snapshot(std::string name) const {
    const std::size_t nx = nodes.size();
    const std::size_t ny = nodes[0].size();
    const std::size_t nz = nodes[0][0].size();

    // Сетка в терминах VTK
    vtkSmartPointer<vtkStructuredGrid> structuredGrid =
        vtkSmartPointer<vtkStructuredGrid>::New();
    // Точки сетки в терминах VTK
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Потенциал
    auto phi = vtkSmartPointer<vtkDoubleArray>::New();
    phi->SetName("phi");

    // Напряжённость электростатического поля
    auto E = vtkSmartPointer<vtkDoubleArray>::New();
    E->SetName("E");
    E->SetNumberOfComponents(3);

    // Индукция магнитного поля
    auto B = vtkSmartPointer<vtkDoubleArray>::New();
    B->SetName("B");
    B->SetNumberOfComponents(3);

    // Градиент потенциала
    auto grad = vtkSmartPointer<vtkDoubleArray>::New();
    grad->SetName("grad");
    grad->SetNumberOfComponents(3);

    // Разница градиента и электростатического поля
    auto diff = vtkSmartPointer<vtkDoubleArray>::New();
    diff->SetName("diff");
    diff->SetNumberOfComponents(3);

    for (unsigned int i = 0; i < nx; ++i) {
        for (unsigned int j = 0; j < ny; ++j) {
            for (unsigned int k = 0; k < nz; ++k) {
                vec3d r     = nodes[i][j][k].getLoc();
                vec3d vE    = nodes[i][j][k].getE();
                vec3d vGrad = nodes[i][j][k].getGrad();
                vec3d vDiff = vE + vGrad;
                vec3d vB    = nodes[i][j][k].getB();
                points->InsertNextPoint(r.x, r.y, r.z);

                double Et[3] = {vE.x, vE.y, vE.z};
                E->InsertNextTuple(Et);

                double Bt[3] = {vB.x, vB.y, vB.z};
                B->InsertNextTuple(Bt);

                phi->InsertNextValue(nodes[i][j][k].getPhi());

                double _grad[3] = {-vGrad.x, -vGrad.y, -vGrad.z};
                grad->InsertNextTuple(_grad);

                double _diff[3] = {vDiff.x, vDiff.y, vDiff.z};
                diff->InsertNextTuple(_diff);
            }
        }
        cout << "Nodes saved: "
             << (i + 1) * nodes[0].size() * nodes[0][0].size() << " out of "
             << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;
    }

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

    // Записываем
    vtkSmartPointer<vtkXMLStructuredGridWriter> writer =
        vtkSmartPointer<vtkXMLStructuredGridWriter>::New();
    const std::string& fileName = name;
    writer->SetFileName(fileName.c_str());
    writer->SetInputData(structuredGrid);
    writer->Write();
}

void CalcMesh::snapshot(std::string                          name,
                        const std::vector<ConductorElement>& conductor) const {
    // Проводник
    vtkSmartPointer<vtkUnstructuredGrid> conductor_mesh =
        vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> conductor_points =
        vtkSmartPointer<vtkPoints>::New();

    // Ток
    auto conductor_I = vtkSmartPointer<vtkDoubleArray>::New();
    conductor_I->SetName("I");
    conductor_I->SetNumberOfComponents(3);

    auto conductor_E = vtkSmartPointer<vtkDoubleArray>::New();
    conductor_E->SetName("E");
    conductor_E->SetNumberOfComponents(3);

    auto conductor_B = vtkSmartPointer<vtkDoubleArray>::New();
    conductor_B->SetName("B");
    conductor_B->SetNumberOfComponents(3);

    for (std::size_t i = 0; i < conductor.size(); ++i) {
        vec3d r = conductor[i].getLoc();
        vec3d I = conductor[i].getI();

        conductor_points->InsertNextPoint(r.x, r.y, r.z);

        double It[3] = {I.x, I.y, I.z};
        conductor_I->InsertNextTuple(It);

        double Et[3] = {0, 0, 0};
        conductor_E->InsertNextTuple(Et);

        double Bt[3] = {0, 0, 0};
        conductor_B->InsertNextTuple(Bt);
    }

    // Создание сетки для проводника
    conductor_mesh->SetPoints(conductor_points);
    conductor_mesh->GetPointData()->AddArray(conductor_I);
    conductor_mesh->GetPointData()->AddArray(conductor_E);
    conductor_mesh->GetPointData()->AddArray(conductor_B);

    const std::size_t nx = nodes.size();
    const std::size_t ny = nodes[0].size();
    const std::size_t nz = nodes[0][0].size();

    // Сетка в терминах VTK
    vtkSmartPointer<vtkStructuredGrid> mesh =
        vtkSmartPointer<vtkStructuredGrid>::New();
    // Точки сетки в терминах VTK
    vtkSmartPointer<vtkPoints> mesh_points = vtkSmartPointer<vtkPoints>::New();

    // Напряжённость электростатического поля
    auto mesh_E = vtkSmartPointer<vtkDoubleArray>::New();
    mesh_E->SetName("E");
    mesh_E->SetNumberOfComponents(3);

    // Индукция магнитного поля
    auto mesh_B = vtkSmartPointer<vtkDoubleArray>::New();
    mesh_B->SetName("B");
    mesh_B->SetNumberOfComponents(3);

    auto mesh_I = vtkSmartPointer<vtkDoubleArray>::New();
    mesh_I->SetName("I");
    mesh_I->SetNumberOfComponents(3);

    for (unsigned int i = 0; i < nx; ++i) {
        for (unsigned int j = 0; j < ny; ++j) {
            for (unsigned int k = 0; k < nz; ++k) {
                vec3d r = nodes[i][j][k].getLoc();
                vec3d E = nodes[i][j][k].getE();
                vec3d B = nodes[i][j][k].getB();

                mesh_points->InsertNextPoint(r.x, r.y, r.z);

                double Et[3] = {E.x, E.y, E.z};
                mesh_E->InsertNextTuple(Et);

                double Bt[3] = {B.x, B.y, B.z};
                mesh_B->InsertNextTuple(Bt);

                double It[3] = {0, 0, 0};
                mesh_I->InsertNextTuple(It);
            }
        }
        cout << "Nodes saved: "
             << (i + 1) * nodes[0].size() * nodes[0][0].size() << " out of "
             << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;
    }

    // Размер и точки сетки
    mesh->SetDimensions(static_cast<int>(nx),
                        static_cast<int>(ny),
                        static_cast<int>(nz));
    mesh->SetPoints(mesh_points);

    // Прикрепление данных
    mesh->GetPointData()->AddArray(mesh_E);
    mesh->GetPointData()->AddArray(mesh_B);
    mesh->GetPointData()->AddArray(mesh_I);

    vtkSmartPointer<vtkAppendFilter> append =
        vtkSmartPointer<vtkAppendFilter>::New();
    append->AddInputData(mesh);
    append->AddInputData(conductor_mesh);
    append->Update();

    // Записываем
    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
        vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    const std::string& fileName = name;
    writer->SetFileName(fileName.c_str());
    writer->SetInputData(append->GetOutput());
    writer->Write();
}
