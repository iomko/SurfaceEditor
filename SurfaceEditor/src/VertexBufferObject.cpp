#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject()
{
	glGenBuffers(1, &id);
	bind();
}

void VertexBufferObject::createData(const void* data, int size, GLenum usage)
{
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VertexBufferObject::updateData(const void* data, int size, int offset)
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(1, &id);
}

void VertexBufferObject::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBufferObject::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
