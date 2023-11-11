#ifndef ELEMENTBUFFEROBJECT_H
#define ELEMENTBUFFEROBJECT_H
#include "glad/glad.h"

class ElementBufferObject {
public:
    ElementBufferObject();
    ~ElementBufferObject();
    void bind();
    void unbind();
    void createData(unsigned int* indices, int count, GLenum usage);
    void updateData(unsigned int* indices, int size, int offset);

private:
    unsigned int id = 0;
};

#endif