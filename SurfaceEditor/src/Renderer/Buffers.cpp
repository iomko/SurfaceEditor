module Renderer.Buffers;
#include "glad/glad.h"
//VertexArrayObject


void VertexArrayObject::create()
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

//VertexBufferObject
void VertexBufferObject::create()
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

//ElementBufferObject


void ElementBufferObject::create()
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






