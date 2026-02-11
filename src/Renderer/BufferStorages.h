#pragma once
#include "BufferStorageData.h"

#include "Buffers.h"
#include <vector>


class LineBufferStorage {
public:
    BufferStorageData<BufferStorageDataType::LineVertex> data;

    void update() {
        data.vbo.bind();
        data.vbo.createData(data.vertices.data(), data.vertices.size() * sizeof(BufferStorageDataType::LineVertex), GL_DYNAMIC_DRAW);
        data.vbo.unbind();
    }

    void create() {
        data.vao.create();
        data.vbo.create();

        data.vao.bind();
        data.vbo.bind();
        data.vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(BufferStorageDataType::LineVertex), (void*)offsetof(BufferStorageDataType::LineVertex, position));
        data.vao.addVertexBufferLayout(1, 1, GL_FLOAT, GL_FALSE, sizeof(BufferStorageDataType::LineVertex), (void*)offsetof(BufferStorageDataType::LineVertex, isHighlighted));
        data.vao.unbind();
        data.vbo.unbind();
    }
};

class TriangleBufferStorage {
public:
    BufferStorageData<BufferStorageDataType::TriangleVertex> data;

    void update() {
        data.vbo.bind();
        data.vbo.createData(data.vertices.data(), data.vertices.size() * sizeof(BufferStorageDataType::TriangleVertex), GL_DYNAMIC_DRAW);
        data.vbo.unbind();
    }

    void create() {
        data.vao.create();
        data.vbo.create();
        
        data.vao.bind();
        data.vbo.bind();
        data.vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(BufferStorageDataType::TriangleVertex), (void*)offsetof(BufferStorageDataType::TriangleVertex, position));
        data.vao.addVertexBufferLayout(1, 3, GL_FLOAT, GL_FALSE, sizeof(BufferStorageDataType::TriangleVertex), (void*)offsetof(BufferStorageDataType::TriangleVertex, normal));
        data.vao.addVertexBufferLayout(2, 1, GL_FLOAT, GL_FALSE, sizeof(BufferStorageDataType::TriangleVertex), (void*)offsetof(BufferStorageDataType::TriangleVertex, isHighlited));
        data.vao.addVertexBufferLayout(3, 1, GL_FLOAT, GL_FALSE, sizeof(BufferStorageDataType::TriangleVertex), (void*)offsetof(BufferStorageDataType::TriangleVertex, isSkewed));
        data.vao.unbind();
        data.vbo.unbind();
    }
};
