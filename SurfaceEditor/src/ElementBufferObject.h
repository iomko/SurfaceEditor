#ifndef ELEMENTBUFFEROBJECT_H
#define ELEMENTBUFFEROBJECT_H
#include "glad/glad.h"

class ElementBufferObject {
public:
    ElementBufferObject(unsigned int* indices, int count, GLenum usage);
    ~ElementBufferObject();
    void bind();
    void unbind();

private:
    unsigned int id = 0;
};

#endif