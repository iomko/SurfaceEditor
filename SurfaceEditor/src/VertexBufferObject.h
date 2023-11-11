#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H
#include "glad/glad.h"

class VertexBufferObject {
public:
    VertexBufferObject();
    void createData(const void* data, int size, GLenum usage);
    void updateData(const void* data, int size, int offset);
    ~VertexBufferObject();

    void bind();
    void unbind();

private:
    unsigned int id = 0;
};
#endif

