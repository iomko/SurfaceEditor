#ifndef MESH_H
#define MESH_H
#include <vector>
#include "TriangleData.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

class Mesh
{
public:
    VertexArrayObject vao;
    VertexBufferObject vbo;
    std::vector<TriangleData> triangles;
    std::vector<float> vertexBufferData;
    Mesh()
    {
        vao.bind();
        vbo.bind();
        vbo.updateData(nullptr, 0, GL_DYNAMIC_DRAW);
        vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        vao.addVertexBufferLayout(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        vao.unbind();
        vbo.unbind();
    }
	void addTriangle(TriangleData triangle)
	{
        vao.bind();
        vbo.bind();
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
        updateBufferData();
        vao.unbind();
        vbo.unbind();
	}
    void render()
    {
        vao.bind();
        vbo.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertexBufferData.size());
        vao.unbind();
        vbo.unbind();
    }

private:
    void updateBufferData()
    {
        vbo.updateData(&vertexBufferData[0], vertexBufferData.size() * sizeof(float), GL_DYNAMIC_DRAW);
    }
};

#endif