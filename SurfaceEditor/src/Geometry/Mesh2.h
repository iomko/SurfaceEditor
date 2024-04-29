#pragma once
#include <vector>
#include "Triangle.h"
#include "../Renderer/VertexDataStructs.h"

class Mesh2
{
private:
	//provizorna trieda, bude obsahovat dalsie potrebne funkcionality akonahle tam pridam HalfEdge data strukturu
    std::vector<Triangle> m_triangles;
    std::vector<MeshVertex> m_vertexBufferData;
    glm::vec3 m_color;
public:
    virtual ~Mesh2() = default;
    Mesh2(glm::vec3 color) : m_color(color){}
    void addTriangle(const Triangle& triangle)
    {
        m_triangles.push_back(triangle);
        for (const auto& position : triangle.m_positions)
        {
            m_vertexBufferData.push_back({position, m_color});
        }
    }
    void clearTriangles()
    {
        m_triangles.clear();
    }

    void clearVertexBufferData()
    {
        m_vertexBufferData.clear();
    }

    std::vector<MeshVertex>& getVertexBufferData()
    {
        return m_vertexBufferData;
    }

    std::vector<Triangle>& getTriangleData()
    {
        return m_triangles;
    }
    virtual void calculateData() = 0;
};
