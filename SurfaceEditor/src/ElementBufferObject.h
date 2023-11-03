#ifndef ELEMENTBUFFEROBJECT_H
#define ELEMENTBUFFEROBJECT_H
#include "glad/glad.h"

class ElementBufferObject {
public:
    ElementBufferObject();
    ~ElementBufferObject();
    void bind();
    void unbind();
    void updateData(unsigned int* indices, int count, GLenum usage);

private:
    unsigned int id = 0;
};

#endif