#ifndef RENDERER_H
#define RENDERER_H
#include "AABBBoundingRegion.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "Mesh.h"

struct AABBVertex {
	glm::vec3 position;
	glm::vec3 color;
};

struct MeshVertex
{
	glm::vec3 position;
	glm::vec3 color;
};

struct LineVertex {
	glm::vec3 position;
	glm::vec3 color;
};

struct RendererData
{
	//LINE
	VertexArrayObject Line_vao;
	VertexBufferObject Line_vbo;

	//MESH
	VertexArrayObject Mesh_vao;
	VertexBufferObject Mesh_vbo;

	//AABB
	VertexArrayObject AABB_vao;
	VertexBufferObject AABB_vbo;
	ElementBufferObject AABB_ebo;

	static const unsigned int maxBoxCount = 2000000;
	static const unsigned int maxVertexCount = 36 * maxBoxCount;
	static const unsigned int maxIndexCount = 24 * maxBoxCount;
	//unsigned int* AABB_indices = new unsigned int[maxIndexCount] {};

	std::vector<AABBVertex> boundingBoxVertices;

	bool onlyOnce = false;
};

class Renderer
{
private:
	RendererData data;
public:
	void init()
	{
		unsigned int offset = 0;
		//indices
		/*
		for(unsigned int i = 0; i < data.maxIndexCount; i +=24)
		{
			//Left face
			data.AABB_indices[i + 0] = 0 + offset;
			data.AABB_indices[i + 1] = 1 + offset;
			data.AABB_indices[i + 2] = 1 + offset;
			data.AABB_indices[i + 3] = 2 + offset;
			data.AABB_indices[i + 4] = 2 + offset;
			data.AABB_indices[i + 5] = 3 + offset;
			data.AABB_indices[i + 6] = 3 + offset;
			data.AABB_indices[i + 7] = 0 + offset;

			// Right face
			data.AABB_indices[i + 8] = 4 + offset;
			data.AABB_indices[i + 9] = 5 + offset;
			data.AABB_indices[i + 10] = 5 + offset;
			data.AABB_indices[i + 11] = 6 + offset;
			data.AABB_indices[i + 12] = 6 + offset;
			data.AABB_indices[i + 13] = 7 + offset;
			data.AABB_indices[i + 14] = 7 + offset;
			data.AABB_indices[i + 15] = 4 + offset;

			// Vertical edges
			data.AABB_indices[i + 16] = 0 + offset;
			data.AABB_indices[i + 17] = 4 + offset;
			data.AABB_indices[i + 18] = 1 + offset;
			data.AABB_indices[i + 19] = 5 + offset;
			data.AABB_indices[i + 20] = 2 + offset;
			data.AABB_indices[i + 21] = 6 + offset;
			data.AABB_indices[i + 22] = 3 + offset;
			data.AABB_indices[i + 23] = 7 + offset;

			offset += 8;
		}
		*/

		//LINE
		data.Line_vao.bind();
		data.Line_vbo.bind();
		data.Line_vbo.createData(nullptr, 2 * sizeof(LineVertex), GL_DYNAMIC_DRAW);
		data.Line_vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)0);
		data.Line_vao.addVertexBufferLayout(1, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, color));
		data.Line_vao.unbind();
		data.Line_vbo.unbind();

		//MESH
		data.Mesh_vao.bind();
		data.Mesh_vbo.bind();
		//1000 trojuholnikov byDefault
		data.Mesh_vbo.createData(nullptr, (3*1000) * sizeof(MeshVertex), GL_DYNAMIC_DRAW);
		data.Mesh_vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)0);
		data.Mesh_vao.addVertexBufferLayout(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, color));
		data.Mesh_vao.unbind();
		data.Mesh_vbo.unbind();
		
		//AABB
		data.AABB_vao.bind();
		data.AABB_vbo.bind();
		//data.AABB_ebo.bind();
		data.AABB_vbo.createData(nullptr, data.maxVertexCount * sizeof(AABBVertex), GL_DYNAMIC_DRAW);

		//data.AABB_ebo.createData(data.AABB_indices, data.maxIndexCount, GL_STATIC_DRAW);


		//data.AABB_ebo.createData(data.AABB_indices, sizeof(data.AABB_indices), GL_STATIC_DRAW);
		data.AABB_vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(AABBVertex), (void*)0);
		data.AABB_vao.addVertexBufferLayout(1, 3, GL_FLOAT, GL_FALSE, sizeof(AABBVertex), (void*)offsetof(AABBVertex, color));
		data.AABB_vao.unbind();
		data.AABB_vbo.unbind();
		//data.AABB_ebo.unbind();
	}

	void collectAABBdata(const AABBBoundingRegion& aabb, const glm::vec3& vertexColor)
	{
		//vertices from top point of view
		//left side
		glm::vec3 bottomUpLeft = aabb.getMin();
		glm::vec3 bottomDownLeft = glm::vec3(aabb.getMin().x, aabb.getMin().y, aabb.getMax().z);
		glm::vec3 upperDownLeft = glm::vec3(aabb.getMin().x, aabb.getMax().y, aabb.getMax().z);
		glm::vec3 upperUpLeft = glm::vec3(aabb.getMin().x, aabb.getMax().y, aabb.getMin().z);
		//right side
		glm::vec3 bottomUpRight = glm::vec3(aabb.getMax().x, aabb.getMin().y, aabb.getMin().z);
		glm::vec3 bottomDownRight = glm::vec3(aabb.getMax().x, aabb.getMin().y, aabb.getMax().z);
		glm::vec3 upperDownRight = aabb.getMax();
		glm::vec3 upperUpRight = glm::vec3(aabb.getMax().x, aabb.getMax().y, aabb.getMin().z);


		//creating these when specifying indices
		/*
		//Constructing AABB vertices
		AABBVertex AABB_vertices[] = {
			{bottomUpLeft, vertexColor},
			{bottomDownLeft, vertexColor},
			{upperDownLeft, vertexColor},
			{upperUpLeft, vertexColor},
			{bottomUpRight, vertexColor},
			{bottomDownRight, vertexColor},
			{upperDownRight, vertexColor},
			{upperUpRight, vertexColor}
		};
		*/

		AABBVertex AABB_vertices[] = {
			//
			{bottomUpLeft, vertexColor},
			{bottomDownLeft, vertexColor},
			{upperDownLeft, vertexColor},
			{upperDownLeft, vertexColor},
			{upperUpLeft, vertexColor},
			{bottomUpLeft, vertexColor},

			//
			
			{upperUpLeft, vertexColor},
			{upperDownLeft, vertexColor},
			{upperDownRight, vertexColor},
			{upperDownRight, vertexColor},
			{upperUpRight, vertexColor},
			{upperUpLeft, vertexColor},

			//
			{upperUpRight, vertexColor},
			{upperDownRight, vertexColor},
			{bottomDownRight, vertexColor},
			{bottomDownRight, vertexColor},
			{bottomUpRight, vertexColor},
			{upperUpRight, vertexColor},

			//
			{bottomUpRight, vertexColor},
			{bottomDownRight, vertexColor},
			{bottomDownLeft, vertexColor},
			{bottomDownLeft, vertexColor},
			{bottomUpLeft, vertexColor},
			{bottomUpRight, vertexColor},

			//
			{upperUpLeft, vertexColor},
			{upperUpRight, vertexColor},
			{bottomUpRight, vertexColor},
			{bottomUpRight, vertexColor},
			{bottomUpLeft, vertexColor},
			{upperUpLeft, vertexColor},
			
			//
			{upperDownLeft, vertexColor},
			{upperDownRight, vertexColor},
			{bottomDownRight, vertexColor},
			{bottomDownRight, vertexColor},
			{bottomDownLeft, vertexColor},
			{upperDownLeft, vertexColor}
			
		};

		// Append AABB vertices to the vector
		data.boundingBoxVertices.insert(data.boundingBoxVertices.end(), std::begin(AABB_vertices), std::end(AABB_vertices));

		/*
		//Binding VAO,VBO,EBO
		data.AABB_vao.bind();
		data.AABB_vbo.bind();
		data.AABB_ebo.bind();
		//Updade VBO
		data.AABB_vbo.updateData(AABB_vertices, std::size(AABB_vertices) * sizeof(AABBVertex), 0);

		//Draw AABB using indices
		glDrawElements(GL_LINES, std::size(data.AABB_indices), GL_UNSIGNED_INT, 0);
		*/
		//Unbinding VAO,VBO,EBO
		//data.AABB_vao.unbind();
		//data.AABB_vbo.unbind();
		//data.AABB_ebo.unbind();
	}

	void drawBoundingBoxes()
	{
		// Binding VAO, VBO, EBO
		data.AABB_vao.bind();
		data.AABB_vbo.bind();
		//data.AABB_ebo.bind();

		// Update VBO and EBO
		if(data.onlyOnce == false)
		{
			data.AABB_vbo.updateData(data.boundingBoxVertices.data(), data.boundingBoxVertices.size() * sizeof(AABBVertex), 0);
		}
		data.onlyOnce = true;

		// Draw all AABBs in a single call
		//glDrawElements(GL_LINES, data.maxIndexCount, GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, data.boundingBoxVertices.size());
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		// Unbinding VAO, VBO, EBO
		//data.AABB_vao.unbind();
		//data.AABB_vbo.unbind();
		//data.AABB_ebo.unbind();

		// Clear the bounding box data for the next frame
		//data.boundingBoxVertices.clear();
		//data.boundingBoxIndices.clear();
	}

	void drawMesh(const Mesh& mesh)
	{
		//Binding VAO,VBO
		data.Mesh_vao.bind();
		data.Mesh_vbo.bind();
		//Updade VBO
		data.Mesh_vbo.updateData(mesh.vertexBufferData.data(), mesh.vertexBufferData.size() * sizeof(float), 0);
		//Draw Mesh using vertexData
		glDrawArrays(GL_TRIANGLES, 0, mesh.vertexBufferData.size()/6);
		//Unbinding VAO,VBO
		data.Mesh_vao.unbind();
		data.Mesh_vbo.unbind();
	}

	void drawLine(const glm::vec3& startPoint, const glm::vec3& endPoint) {
		
		LineVertex lineVertex_vertices[] = {
			{startPoint, glm::vec3(0.0f, 0.0f, 1.0f)},
			{endPoint, glm::vec3(0.0f, 0.0f, 1.0f)}
		};

		//Binding VAO,VBO
		data.Line_vao.bind();
		data.Line_vbo.bind();
		//Updade VBO
		data.Line_vbo.updateData(&lineVertex_vertices[0], std::size(lineVertex_vertices) * sizeof(LineVertex), 0);
		//Draw Mesh using vertexData
		glDrawArrays(GL_LINES, 0, std::size(lineVertex_vertices));
		//Unbinding VAO,VBO
		data.Line_vao.unbind();
		data.Line_vbo.unbind();
	}

	~Renderer()
	{
		//delete[] data.AABB_indices;
	}
};
#endif