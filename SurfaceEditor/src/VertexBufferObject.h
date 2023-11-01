#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H
#include "glad/glad.h"

class VertexBufferObject {
public:
    VertexBufferObject(void* data, int size, GLenum usage);
    ~VertexBufferObject();

    void bind();
    void unbind();

private:
    unsigned int id = 0;
};
#endif

