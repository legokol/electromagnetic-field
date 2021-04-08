#include "CalcMesh.h"

CalcMesh::CalcMesh(unsigned int size, double h) {
    this->h = h;
    nodes.resize(size);
    for (unsigned int i = 0; i < size; ++i) {
        nodes[i].resize(size);
        for (unsigned int j = 0; j < size; ++j) {
            nodes[i][j].resize(size);
            for (unsigned int k = 0; k < size; ++k) {
                // Позиция
                vector3D loc(i * h, j * h, k * h);
                nodes[i][j][k] = CalcNode(loc);
            }
        }
    }
}

CalcMesh::CalcMesh(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, double h) {
    this->h = h;
    nodes.resize(sizeX);
    for (unsigned int i = 0; i < sizeX; ++i) {
        nodes[i].resize(sizeY);
        for (unsigned int j = 0; j < sizeY; ++j) {
            nodes[i][j].resize(sizeZ);
            for (unsigned int k = 0; k < sizeZ; ++k) {
                // Позиция
                vector3D loc(i * h, j * h, k * h);
                nodes[i][j][k] = CalcNode(loc);
            }
        }
    }
}

void CalcMesh::calculate(const ConductorElement &element) {
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        for (unsigned int j = 0; j < nodes[i].size(); ++j) {
            for (unsigned int k = 0; k < nodes[i][j].size(); ++k) {
                vector3D r = nodes[i][j][k].getLoc() - element.getLoc();
                if (abs(r.getX()) > 0 || abs(r.getY()) > 0 || abs(r.getZ()) > 0) {
                    nodes[i][j][k].setE(element.calculateE(r));
                    nodes[i][j][k].setB(element.calculateB(r));
                    nodes[i][j][k].setPhi(element.calculatePhi(r));
                }
            }
            cout << "Nodes calculated: " << (i + 1) * nodes[0].size() * nodes[0][0].size() << " out of "
                 << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;
        }
    }
}

void CalcMesh::calculate(const vector<ConductorElement> &conductor) {
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        for (unsigned int j = 0; j < nodes[i].size(); ++j) {
            for (unsigned int k = 0; k < nodes[i][j].size(); ++k) {
                for (unsigned int s = 0; s < conductor.size(); ++s) {
                    vector3D r = nodes[i][j][k].getLoc() - conductor[s].getLoc();
                    if (abs(r.getX()) > 0 || abs(r.getY()) > 0 || abs(r.getZ()) > 0) {
                        nodes[i][j][k].setE(nodes[i][j][k].getE() + conductor[s].calculateE(r));
                        nodes[i][j][k].setB(nodes[i][j][k].getB() + conductor[s].calculateB(r));
                        nodes[i][j][k].setPhi(nodes[i][j][k].getPhi() + conductor[s].calculatePhi(r));
                    }
                }
            }
        }
        cout << "Nodes calculated: " << (i + 1) * nodes[0].size() * nodes[0][0].size() << " out of "
             << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;
    }
}

void CalcMesh::calculateGrad() {
    for (unsigned int i = 1; i < nodes.size() - 1; ++i) {
        for (unsigned int j = 1; j < nodes[i].size() - 1; ++j) {
            for (unsigned int k = 1; k < nodes[i][j].size() - 1; ++k) {
                vector3D grad;
                grad.setX((nodes[i + 1][j][k].getPhi() - nodes[i - 1][j][k].getPhi()) / (2 * h));
                grad.setY((nodes[i][j + 1][k].getPhi() - nodes[i][j - 1][k].getPhi()) / (2 * h));
                grad.setZ((nodes[i][j][k + 1].getPhi() - nodes[i][j][k - 1].getPhi()) / (2 * h));
                nodes[i][j][k].setGrad(grad);
            }
        }
    }
}

void CalcMesh::snapshot(std::string name) const {
    unsigned int sizeX = nodes.size();
    unsigned int sizeY = nodes[0].size();
    unsigned int sizeZ = nodes[0][0].size();

    // Сетка в терминах VTK
    vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();
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

    for (unsigned int i = 0; i < sizeX; ++i) {
        for (unsigned int j = 0; j < sizeY; ++j) {
            for (unsigned int k = 0; k < sizeZ; ++k) {
                vector3D r = nodes[i][j][k].getLoc();
                vector3D vE = nodes[i][j][k].getE();
                vector3D vGrad = nodes[i][j][k].getGrad();
                vector3D vDiff = vE + vGrad;
                vector3D vB = nodes[i][j][k].getB();
                points->InsertNextPoint(r.getX(), r.getY(), r.getZ());

                double _E[3] = {vE.getX(), vE.getY(), vE.getZ()};
                E->InsertNextTuple(_E);

                double _B[3] = {vB.getX(), vB.getY(), vB.getZ()};
                B->InsertNextTuple(_B);

                phi->InsertNextValue(nodes[i][j][k].getPhi());

                double _grad[3] = {-vGrad.getX(), -vGrad.getY(), -vGrad.getZ()};
                grad->InsertNextTuple(_grad);

                double _diff[3] = {vDiff.getX(), vDiff.getY(), vDiff.getZ()};
                diff->InsertNextTuple(_diff);
            }
        }
        cout << "Nodes saved: " << (i + 1) * nodes[0].size() * nodes[0][0].size() << " out of "
             << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;
    }

    // Размер и точки сетки
    structuredGrid->SetDimensions(sizeX, sizeY, sizeZ);
    structuredGrid->SetPoints(points);

    // Прикрепление данных
    structuredGrid->GetPointData()->AddArray(E);
    structuredGrid->GetPointData()->AddArray(B);
    structuredGrid->GetPointData()->AddArray(phi);
    structuredGrid->GetPointData()->AddArray(grad);
    structuredGrid->GetPointData()->AddArray(diff);

    // Записываем
    vtkSmartPointer<vtkXMLStructuredGridWriter> writer = vtkSmartPointer<vtkXMLStructuredGridWriter>::New();
    std::string fileName = name;
    writer->SetFileName(fileName.c_str());
    writer->SetInputData(structuredGrid);
    writer->Write();
}

void CalcMesh::snapshot(std::string name, const vector<ConductorElement> &conductor) const {
    // Проводник
    vtkSmartPointer<vtkUnstructuredGrid> conductor_mesh = vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> conductor_points = vtkSmartPointer<vtkPoints>::New();

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

    for (int i = 0; i < conductor.size(); ++i) {
        vector3D r = conductor[i].getLoc();
        vector3D I = conductor[i].getI();

        conductor_points->InsertNextPoint(r.getX(), r.getY(), r.getZ());

        double _I[3] = {I.getX(), I.getY(), I.getZ()};
        conductor_I->InsertNextTuple(_I);

        double _E[3] = {0, 0, 0};
        conductor_E->InsertNextTuple(_E);

        double _B[3] = {0, 0, 0};
        conductor_B->InsertNextTuple(_B);
    }

    // Создание сетки для проводника
    conductor_mesh->SetPoints(conductor_points);
    conductor_mesh->GetPointData()->AddArray(conductor_I);
    conductor_mesh->GetPointData()->AddArray(conductor_E);
    conductor_mesh->GetPointData()->AddArray(conductor_B);

    unsigned int sizeX = nodes.size();
    unsigned int sizeY = nodes[0].size();
    unsigned int sizeZ = nodes[0][0].size();

    // Сетка в терминах VTK
    vtkSmartPointer<vtkStructuredGrid> mesh = vtkSmartPointer<vtkStructuredGrid>::New();
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

    for (unsigned int i = 0; i < sizeX; ++i) {
        for (unsigned int j = 0; j < sizeY; ++j) {
            for (unsigned int k = 0; k < sizeZ; ++k) {
                vector3D r = nodes[i][j][k].getLoc();
                vector3D E = nodes[i][j][k].getE();
                vector3D B = nodes[i][j][k].getB();

                mesh_points->InsertNextPoint(r.getX(), r.getY(), r.getZ());

                double _E[3] = {E.getX(), E.getY(), E.getZ()};
                mesh_E->InsertNextTuple(_E);

                double _B[3] = {B.getX(), B.getY(), B.getZ()};
                mesh_B->InsertNextTuple(_B);

                double _I[3] = {0, 0, 0};
                mesh_I->InsertNextTuple(_I);
            }
        }
        cout << "Nodes saved: " << (i + 1) * nodes[0].size() * nodes[0][0].size() << " out of "
             << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;
    }

    // Размер и точки сетки
    mesh->SetDimensions(sizeX, sizeY, sizeZ);
    mesh->SetPoints(mesh_points);

    // Прикрепление данных
    mesh->GetPointData()->AddArray(mesh_E);
    mesh->GetPointData()->AddArray(mesh_B);
    mesh->GetPointData()->AddArray(mesh_I);

    vtkSmartPointer<vtkAppendFilter> append = vtkSmartPointer<vtkAppendFilter>::New();
    append->AddInputData(mesh);
    append->AddInputData(conductor_mesh);
    append->Update();

    // Записываем
    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    std::string fileName = name;
    writer->SetFileName(fileName.c_str());
    writer->SetInputData(append->GetOutput());
    writer->Write();
}
