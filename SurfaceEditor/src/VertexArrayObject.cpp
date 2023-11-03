#include "VertexArrayObject.h"

#include "glad/glad.h"

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &id);
	bind();
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &id);
}

void VertexArrayObject::bind()
{
	glBindVertexArray(id);
}

void VertexArrayObject::unbind()
{
	glBindVertexArray(0);
}

void VertexArrayObject::addVertexBufferLayout(int index, int size, GLenum type, bool normalized, int stride, void* offset)
{
	glVertexAttribPointer(index, size, type, normalized, stride, offset);
	glEnableVertexAttribArray(index);
}
