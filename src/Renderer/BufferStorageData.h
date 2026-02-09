#pragma once
#include "../Renderer/Buffers.h"
#include <vector>
#include <glm/glm.hpp>

struct BufferStorageDataType 
{
    struct TriangleVertex 
    {
        glm::vec3 position;
        glm::vec3 normal;
        float isHighlited = 0.0f;
        float isSkewed = 0.0f;
    };

	struct AABBVertex {
		glm::vec3 position;
		glm::vec3 color;
	};

	struct LineVertex {
		glm::vec3 position;
		float isHighlighted;
	};

	struct PointVertex
	{
		glm::vec3 position;
		float isHighlighted = 0.0f;
	};

	struct MeshVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		float isHighlited = 0.0f;
	};
};

template<typename DataType>
struct BufferStorageData {
    VertexArrayObject vao;
    VertexBufferObject vbo;
    std::vector<DataType> vertices;
};
