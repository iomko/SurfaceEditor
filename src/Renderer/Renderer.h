#pragma once
#include <vector>

#include "Buffers.h"
#include <string>
#include "Structures/PrintableMesh.h"
#include "BufferStorageData.h"
#include "../ViewPortsController.h"


class Renderer {
private:

    using AABBVertex = BufferStorageDataType::AABBVertex;
    using LineVertex = BufferStorageDataType::LineVertex;
    using PointVertex = BufferStorageDataType::PointVertex;
    using MeshVertex = BufferStorageDataType::MeshVertex;
    using TriangleVertex = BufferStorageDataType::TriangleVertex;

public:

    static void drawTriangles(BufferStorageData<BufferStorageDataType::TriangleVertex>& bufferData, Shader* shader, Mesh* mesh = nullptr) {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);
        
        shader->bind();
        if (mesh != nullptr)
        {
            shader->setMat4("u_model", mesh->m_transform);
            shader->setMat4("u_view", ViewPortsHolderContext::s_camera->m_matrices.viewMatrix);
            shader->setMat4("u_projection", ViewPortsHolderContext::s_camera->m_matrices.perspectiveMatrix);
        }
        bufferData.vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, bufferData.vertices.size());
        bufferData.vao.unbind();
        shader->unbind();

        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    static void drawLines(BufferStorageData<BufferStorageDataType::LineVertex>& bufferData, Shader* shader, Mesh* mesh = nullptr) {
        shader->bind();
        if (mesh != nullptr)
        {
            shader->setMat4("u_model", mesh->m_transform);
            shader->setMat4("u_view", ViewPortsHolderContext::s_camera->m_matrices.viewMatrix);
            shader->setMat4("u_projection", ViewPortsHolderContext::s_camera->m_matrices.perspectiveMatrix);
        }
        bufferData.vao.bind();
        glLineWidth(3.0f);
        glDrawArrays(GL_LINES, 0, bufferData.vertices.size());
        glLineWidth(0.5f);
        bufferData.vao.unbind();
        shader->unbind();
    }
};


