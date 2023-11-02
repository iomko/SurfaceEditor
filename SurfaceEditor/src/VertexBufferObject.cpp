#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject()
{
	glGenBuffers(1, &id);
}

void VertexBufferObject::updateData(void* data, int size, GLenum usage)
{
	//bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	//unbind();
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
