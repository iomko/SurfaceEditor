#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject(void* data, int size, GLenum usage)
{
	glGenBuffers(1, &id);
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
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
