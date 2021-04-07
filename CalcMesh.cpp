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
    std::ofstream out("Field.txt");
    //cout << "Calculation started" << endl;
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        for (unsigned int j = 0; j < nodes[i].size(); ++j) {
            for (unsigned int k = 0; k < nodes[i][j].size(); ++k) {
                vector3D r = nodes[i][j][k].getLoc() - element.getLoc();
                if (abs(r.getX()) > 0 || abs(r.getY()) > 0 || abs(r.getZ()) > 0) {
                    vector3D E = element.calculateE(r);
                    if (E.magnitude() > 1)
                        out << r.magnitude() << ' ' << E.magnitude() << std::endl;
                    nodes[i][j][k].setE(element.calculateE(r));
                    //nodes[i][I][k].setB(element.calculateB(r));
                    nodes[i][j][k].setPhi(element.calculatePhi(r));
                }
            }
            /*cout << "Nodes calculated " << (i + 1) * (I + 1) * nodes[i][I].size() << '/'
                 << nodes.size() * nodes[0].size() * nodes[0][0].size() << endl;*/
        }
    }
}

void CalcMesh::calculate(const vector<ConductorElement> &conductor) {
    for (unsigned int s = 0; s < conductor.size(); ++s) {
        for (unsigned int i = 0; i < nodes.size(); ++i) {
            for (unsigned int j = 0; j < nodes[i].size(); ++j) {
                for (unsigned int k = 0; k < nodes[i][j].size(); ++k) {
                    vector3D r = nodes[i][j][k].getLoc() - conductor[s].getLoc();
                    if (abs(r.getX()) > 0 || abs(r.getY()) > 0 || abs(r.getZ()) > 0) {
                        nodes[i][j][k].setE(nodes[i][j][k].getE() + conductor[s].calculateE(r));
                        //nodes[i][I][k].setB(conductor[s].calculateB(r));
                        nodes[i][j][k].setPhi(nodes[i][j][k].getPhi() + conductor[s].calculatePhi(r));
                    }
                }
            }
        }
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
    /*auto B = vtkSmartPointer<vtkDoubleArray>::New();
    B->SetName("B");
    B->SetNumberOfComponents(3);*/

    // Градиент потенциала
    auto grad = vtkSmartPointer<vtkDoubleArray>::New();
    grad->SetName("grad");
    grad->SetNumberOfComponents(3);

    // Разница градиента и электростатического поля
    auto diff = vtkSmartPointer<vtkDoubleArray>::New();
    diff->SetName("diff");
    diff->SetNumberOfComponents(3);

    unsigned int sizeX = nodes.size();
    unsigned int sizeY = nodes[0].size();
    unsigned int sizeZ = nodes[0][0].size();

    for (unsigned int i = 0; i < sizeX; ++i) {
        for (unsigned int j = 0; j < sizeY; ++j) {
            for (unsigned int k = 0; k < sizeZ; ++k) {
                vector3D r = nodes[i][j][k].getLoc();
                vector3D vE = nodes[i][j][k].getE();
                vector3D vGrad = nodes[i][j][k].getGrad();
                vector3D vDiff = vE + vGrad;
                //vector3D vB = nodes[i][I][k].getB();
                points->InsertNextPoint(r.getX(), r.getY(), r.getZ());

                double _E[3] = {vE.getX(), vE.getY(), vE.getZ()};
                E->InsertNextTuple(_E);

                /*double _B[3] = {vB.getX(), vB.getY(), vB.getZ()};
                B->InsertNextTuple(_B);*/

                phi->InsertNextValue(nodes[i][j][k].getPhi());

                double _grad[3] = {-vGrad.getX(), -vGrad.getY(), -vGrad.getZ()};
                grad->InsertNextTuple(_grad);

                double _diff[3] = {vDiff.getX(), vDiff.getY(), vDiff.getZ()};
                diff->InsertNextTuple(_diff);
            }
        }
    }

    // Размер и точки сетки
    structuredGrid->SetDimensions(sizeX, sizeY, sizeZ);
    structuredGrid->SetPoints(points);

    // Прикрепление данных
    structuredGrid->GetPointData()->AddArray(E);
    //structuredGrid->GetPointData()->AddArray(B);
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