#include "ElementBufferObject.h"


ElementBufferObject::ElementBufferObject(unsigned int* indices, int count, GLenum usage)
{
	glGenBuffers(1, &id);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, usage);
}

ElementBufferObject::~ElementBufferObject() {
	glDeleteBuffers(1, &id);
}

void ElementBufferObject::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void ElementBufferObject::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


