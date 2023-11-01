#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H
#include "glad/glad.h"

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();
	void bind();
	void unbind();
	void addVertexBufferLayout(int index, int size, GLenum type, bool normalized, int stride, void* offset);

private:
	unsigned int id = 0;
};

#endif
