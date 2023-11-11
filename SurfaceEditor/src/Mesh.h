#ifndef MESH_H
#define MESH_H
#include <vector>
#include "TriangleData.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

class Mesh
{
public:
    std::vector<TriangleData> triangles;
    std::vector<float> vertexBufferData;
    Mesh()
    {
    }
    void addTriangle(TriangleData triangle)
    {
        this->triangles.push_back(triangle);

        for (const auto& position : triangle.positions)
        {
            vertexBufferData.push_back(position.x);
            vertexBufferData.push_back(position.y);
            vertexBufferData.push_back(position.z);
            vertexBufferData.push_back(triangle.color.r);
            vertexBufferData.push_back(triangle.color.g);
            vertexBufferData.push_back(triangle.color.b);
        }
    }
};

#endif