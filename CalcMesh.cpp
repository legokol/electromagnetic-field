#include "CalcMesh.h"

CalcMesh::CalcMesh(unsigned int size, double h) {
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
