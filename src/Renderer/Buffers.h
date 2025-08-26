#pragma once
#include <glad/gl.h>

class VertexArrayObject
{
public:
	VertexArrayObject() = default;

	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(const VertexArrayObject&) = delete;

	VertexArrayObject(VertexArrayObject&& other) noexcept
		: id(other.id)
	{
		other.id = 0;
	}

	VertexArrayObject& operator=(VertexArrayObject&& other) noexcept
	{
		if (this != &other)
		{
			if (id != 0)
				glDeleteVertexArrays(1, &id);

			id = other.id;
			other.id = 0;
		}
		return *this;
	}

	~VertexArrayObject()
	{
		glDeleteVertexArrays(1, &id);
	}

	void bind()
	{
		glBindVertexArray(id);
	}
	void unbind()
	{
		glBindVertexArray(0);
	}
	void create()
	{
		glGenVertexArrays(1, &id);
		bind();
	}
	void addVertexBufferLayout(int index, int size, GLenum type, bool normalized, int stride, void* offset)
	{
		glVertexAttribPointer(index, size, type, normalized, stride, offset);
		glEnableVertexAttribArray(index);
	}
public:
	unsigned int id = 0;
};

class VertexBufferObject {
public:
	VertexBufferObject() = default;

	VertexBufferObject(const VertexBufferObject&) = delete;
	VertexBufferObject& operator=(const VertexBufferObject&) = delete;

	VertexBufferObject(VertexBufferObject&& other) noexcept
		: id(other.id)
	{
		other.id = 0;
	}

	VertexBufferObject& operator=(VertexBufferObject&& other) noexcept
	{
		if (this != &other)
		{
			if (id != 0)
				glDeleteBuffers(1, &id);

			id = other.id;
			other.id = 0;
		}
		return *this;
	}

	~VertexBufferObject()
	{
		glDeleteBuffers(1, &id);
	}

	void bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}
	void unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void create()
	{
		glGenBuffers(1, &id);
		bind();
	}
	void createData(const void* data, int size, GLenum usage)
	{
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	}
	void updateData(const void* data, int size, int offset)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}
private:
	unsigned int id = 0;
};

class ElementBufferObject {
public:
	ElementBufferObject() = default;

	ElementBufferObject(const ElementBufferObject&) = delete;
	ElementBufferObject& operator=(const ElementBufferObject&) = delete;

	ElementBufferObject(ElementBufferObject&& other) noexcept
		: id(other.id)
	{
		other.id = 0;
	}

	ElementBufferObject& operator=(ElementBufferObject&& other) noexcept
	{
		if (this != &other)
		{
			if (id != 0)
				glDeleteBuffers(1, &id);

			id = other.id;
			other.id = 0;
		}
		return *this;
	}

	~ElementBufferObject()
	{
		glDeleteBuffers(1, &id);
	}

	void bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}
	void unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void create()
	{
		glGenBuffers(1, &id);
		bind();
	}
	void createData(unsigned int* indices, int count, GLenum usage)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indices, usage);
	}
	void updateData(unsigned int* indices, int size, int offset)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, indices);
	}

private:
	unsigned int id = 0;
};
