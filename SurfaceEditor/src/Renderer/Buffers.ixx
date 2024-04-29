module;
#include "glad/glad.h"
export module Renderer.Buffers;

export class VertexArrayObject
{
public:
	//VertexArrayObject();
	~VertexArrayObject();
    void create();
	void bind();
	void unbind();
	void addVertexBufferLayout(int index, int size, GLenum type, bool normalized, int stride, void* offset);
public:
	unsigned int id = 0;
};

export class VertexBufferObject {
public:
    //VertexBufferObject();
    void create();
    void createData(const void* data, int size, GLenum usage);
    void updateData(const void* data, int size, int offset);
    ~VertexBufferObject();

    void bind();
    void unbind();

private:
    unsigned int id = 0;
};

export class ElementBufferObject {
public:
    void create();
    ~ElementBufferObject();
    void bind();
    void unbind();
    void createData(unsigned int* indices, int count, GLenum usage);
    void updateData(unsigned int* indices, int size, int offset);

private:
    unsigned int id = 0;
};



