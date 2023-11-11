#include "ElementBufferObject.h"


ElementBufferObject::ElementBufferObject()
{
	glGenBuffers(1, &id);
	bind();
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

void ElementBufferObject::createData(unsigned int* indices, int count, GLenum usage)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indices, usage);
}

void ElementBufferObject::updateData(unsigned int* indices, int size, int offset)
{
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, indices);
}




