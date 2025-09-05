#pragma once

#include <optional>

#include "../Primitives/AABB.h"
#include <vector>

#include "Buffers.h"
#include "../Scene/Mesh.h"

struct RendererStageData
{
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

	using MeshLinesMap = std::map<Mesh*, std::vector<LineVertex>>;
	using MatVertsMap = std::map<Material*, std::vector<MeshVertex>>;
	using MeshMatsMap = std::map<Mesh*, MatVertsMap>;
	using AABBVertsMap = std::map<AABBBoundingRegion, std::vector<AABBVertex>>;

	MeshLinesMap meshLinesMap;
	MeshMatsMap meshMatsMap;
	AABBVertsMap aabbVertsMap;
private:
	RendererStageData() = default;

	friend class Renderer;
};

struct RendererConfig
{
	static constexpr unsigned int maxBoxCount = 2000000;
	static constexpr unsigned int maxVertexCount = 36 * maxBoxCount;
	static constexpr unsigned int maxIndexCount = 24 * maxBoxCount;
};

class Renderer {
public:
	static void init()
	{
		using AABBVertex = RendererStageData::AABBVertex;
		using LineVertex = RendererStageData::LineVertex;
		using PointVertex = RendererStageData::PointVertex;
		using MeshVertex = RendererStageData::MeshVertex;

		s_bufferRegistry.registerBuffer<BufferRegistry::BufferType::Line>(false);
		s_bufferRegistry.registerBuffer<BufferRegistry::BufferType::Mesh>(false);
		s_bufferRegistry.registerBuffer<BufferRegistry::BufferType::AABB>(true);
		s_bufferRegistry.registerBuffer<BufferRegistry::BufferType::Point>(false);

		//LINE
		BufferRegistry::Buffer& lineBuffer = s_bufferRegistry.queryBuffer<BufferRegistry::BufferType::Line>();
		
		lineBuffer.vao.bind();
		lineBuffer.vbo.bind();
		lineBuffer.vbo.createData(nullptr, (50000000 * 2) * sizeof(LineVertex), GL_DYNAMIC_DRAW);
		//data.Line_vbo.createData(nullptr, 2 * sizeof(LineVertex), GL_DYNAMIC_DRAW);
		lineBuffer.vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, position));
		lineBuffer.vao.addVertexBufferLayout(1, 1, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, isHighlighted));
		lineBuffer.vao.unbind();
		lineBuffer.vbo.unbind();

		//MESH
		BufferRegistry::Buffer& meshBuffer = s_bufferRegistry.queryBuffer<BufferRegistry::BufferType::Mesh>();
		meshBuffer.vao.bind();
		meshBuffer.vbo.bind();
		meshBuffer.vbo.createData(nullptr, (3 * 1000000) * sizeof(MeshVertex), GL_DYNAMIC_DRAW);
		meshBuffer.vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, position));
		meshBuffer.vao.addVertexBufferLayout(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, normal));
		meshBuffer.vao.addVertexBufferLayout(2, 1, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, isHighlited));
		meshBuffer.vao.unbind();
		meshBuffer.vbo.unbind();

		//POINTS
		BufferRegistry::Buffer& pointBuffer = s_bufferRegistry.queryBuffer<BufferRegistry::BufferType::Point>();
		pointBuffer.vao.bind();
		pointBuffer.vbo.bind();
		pointBuffer.vbo.createData(nullptr, 1000000 * sizeof(PointVertex), GL_DYNAMIC_DRAW);
		pointBuffer.vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointVertex), (void*)0);
		pointBuffer.vao.addVertexBufferLayout(1, 1, GL_FLOAT, GL_FALSE, sizeof(PointVertex), (void*)(offsetof(PointVertex, isHighlighted)));
		pointBuffer.vao.unbind();
		pointBuffer.vbo.unbind();

		//AABB
		BufferRegistry::Buffer& aabbBuffer = s_bufferRegistry.queryBuffer<BufferRegistry::BufferType::AABB>();
		aabbBuffer.vao.bind();
		aabbBuffer.vbo.bind();
		aabbBuffer.vbo.createData(nullptr, RendererConfig::maxVertexCount * sizeof(AABBVertex), GL_DYNAMIC_DRAW);

		aabbBuffer.vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(AABBVertex), (void*)0);
		aabbBuffer.vao.addVertexBufferLayout(1, 3, GL_FLOAT, GL_FALSE, sizeof(AABBVertex), (void*)offsetof(AABBVertex, color));
		aabbBuffer.vao.unbind();
		aabbBuffer.vbo.unbind();
	}
    static void drawPoints(std::vector<RendererStageData::PointVertex>& points)
	{
		BufferRegistry::Buffer& pointBuffer = s_bufferRegistry.queryBuffer<BufferRegistry::BufferType::Point>();
		glDisable(GL_PROGRAM_POINT_SIZE);
		// Binding VAO, VBO, EBO
		pointBuffer.vao.bind();
		pointBuffer.vbo.bind();

		pointBuffer.vbo.updateData(points.data(), points.size() * sizeof(RendererStageData::PointVertex), 0);

		glPointSize(10.0f);

		glDrawArrays(GL_POINTS, 0, points.size());

		// Unbinding VAO, VBO
		pointBuffer.vao.unbind();
		pointBuffer.vbo.unbind();
	}

    static void updateMesh(const std::vector<RendererStageData::MeshVertex>& mesh){
		BufferRegistry::Buffer& meshBuffer = s_bufferRegistry.queryBuffer<BufferRegistry::BufferType::Mesh>();
		// Binding VAO, VBO, EBO
		meshBuffer.vao.bind();
		meshBuffer.vbo.bind();

		meshBuffer.vbo.updateData(mesh.data(), mesh.size() * sizeof(RendererStageData::MeshVertex), 0);

		// Unbinding VAO, VBO
		meshBuffer.vao.unbind();
		meshBuffer.vbo.unbind();
    }

    static void drawMesh(const std::vector<RendererStageData::MeshVertex>& mesh)
    {
		BufferRegistry::Buffer& meshBuffer = s_bufferRegistry.queryBuffer<BufferRegistry::BufferType::Mesh>();

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f); // offset face slightly back

		// Binding VAO, VBO, EBO
		meshBuffer.vao.bind();

		glDrawArrays(GL_TRIANGLES, 0, mesh.size());

		// Unbinding VAO, VBO
		meshBuffer.vao.unbind();

        glDisable(GL_POLYGON_OFFSET_FILL);
    }
    static void drawBox(const std::vector<RendererStageData::AABBVertex>& box)
    {
		BufferRegistry::Buffer& aabbBuffer = s_bufferRegistry.queryBuffer<BufferRegistry::BufferType::AABB>();
		// Binding VAO, VBO, EBO
		aabbBuffer.vao.bind();
		aabbBuffer.vbo.bind();
		//data.AABB_ebo.bind();

		aabbBuffer.vbo.updateData(box.data(), box.size() * sizeof(RendererStageData::AABBVertex), 0);

		// Draw all AABBs in a single call
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, box.size());
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Unbinding VAO, VBO, EBO
		aabbBuffer.vao.unbind();
		aabbBuffer.vbo.unbind();
		aabbBuffer.ebo->unbind();
    }

    static void updateLines(std::vector<RendererStageData::LineVertex>& lines){
		BufferRegistry::Buffer& lineBuffer = s_bufferRegistry.queryBuffer<BufferRegistry::BufferType::Line>();

        lineBuffer.vao.bind();
        lineBuffer.vbo.bind();

        lineBuffer.vbo.updateData(lines.data(), lines.size() * sizeof(RendererStageData::LineVertex), 0);

        lineBuffer.vao.unbind();
        lineBuffer.vbo.unbind();
    }
    static void drawLines(std::vector<RendererStageData::LineVertex>& lines)
    {
		BufferRegistry::Buffer& lineBuffer = s_bufferRegistry.queryBuffer<BufferRegistry::BufferType::Line>();

		//Binding VAO,VBO
		lineBuffer.vao.bind();
		//Draw Mesh using vertexData
		glLineWidth(1.0f);
		glDrawArrays(GL_LINES, 0, lines.size());
		glLineWidth(0.5f);
		//Unbinding VAO,VBO
		lineBuffer.vao.unbind();
    }

private:
	class BufferRegistry
	{
	public:
		enum class BufferType
		{
			Point, Line, Mesh, AABB, ScreenQuad
		};

		struct Buffer
		{
			VertexArrayObject vao;
			VertexBufferObject vbo;
			std::optional<ElementBufferObject> ebo;
		};

		template<BufferType T>
		void registerBuffer(bool useEbo) {
			Buffer buffer;
			buffer.vao.create();
			buffer.vbo.create();
			if (useEbo)
			{
				buffer.ebo = ElementBufferObject();
				buffer.ebo->create();
			}
			m_buffers.insert({ T, std::move(buffer) });
		}

		template<BufferType T>
		Buffer& queryBuffer()
		{
			return m_buffers[T];
		}

	private:
		std::map<BufferType, Buffer> m_buffers;
	};
private:
	inline static BufferRegistry s_bufferRegistry;

public:
	inline static RendererStageData s_stageData;
};
