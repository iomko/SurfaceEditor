#include "Renderer.h"
#include "../AABBBoundingRegion.h"
#include "glad/glad.h"

static RendererData data;

void Renderer::init() {
	//create
	//LINE
	data.Line_vao.create();
	data.Line_vbo.create();
	//MESH
	data.Mesh_vao.create();
	data.Mesh_vbo.create();
	//Mesh
	data.Mesh_vao.create();
	data.Mesh_vbo.create();
	//AABB
	data.AABB_vao.create();
	data.AABB_vbo.create();
	//POINTS
	data.Point_vao.create();
	data.Point_vbo.create();


	//LINE
	data.Line_vao.bind();
	data.Line_vbo.bind();
	data.Line_vbo.createData(nullptr, (10000000*2) * sizeof(LineVertex), GL_DYNAMIC_DRAW);
	//data.Line_vbo.createData(nullptr, 2 * sizeof(LineVertex), GL_DYNAMIC_DRAW);
	data.Line_vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, point));
	data.Line_vao.addVertexBufferLayout(1, 1, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, isHighlighted));
	data.Line_vao.unbind();
	data.Line_vbo.unbind();

	//MESH
	data.Mesh_vao.bind();
	data.Mesh_vbo.bind();
	data.Mesh_vbo.createData(nullptr, (3 * 1000000) * sizeof(MeshVertex), GL_DYNAMIC_DRAW);
	data.Mesh_vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, position));
	data.Mesh_vao.addVertexBufferLayout(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, normal));
	data.Mesh_vao.addVertexBufferLayout(2, 1, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, isHighlited));
	data.Mesh_vao.unbind();
	data.Mesh_vbo.unbind();

	//POINTS
	data.Point_vao.bind();
	data.Point_vbo.bind();
	data.Point_vbo.createData(nullptr, 1000000 * sizeof(MeshPoint), GL_DYNAMIC_DRAW);
	data.Point_vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void*)0);
	data.Point_vao.addVertexBufferLayout(1, 1, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void*)(offsetof(MeshPoint, isHighlighted)));
	data.Point_vao.unbind();
	data.Point_vbo.unbind();


	// screen quad
	float cs[] = { -1.0F, 1.0F,  0.0F, 1.0F, -1.0F, -1.0F, 0.0F, 0.0F,
				  1.0F,  -1.0F, 1.0F, 0.0F, -1.0F, 1.0F,  0.0F, 1.0F,
				  1.0F,  -1.0F, 1.0F, 0.0F, 1.0F,  1.0F,  1.0F, 1.0F };
	glGenVertexArrays(1, &RendererData::screenQuadVAO);
	glGenBuffers(1, &RendererData::screenQuadVBO);
	glBindVertexArray(RendererData::screenQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, RendererData::screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cs), &cs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
		(void*)(2 * sizeof(float)));
	glBindVertexArray(0);


	//AABB
	data.AABB_vao.bind();
	data.AABB_vbo.bind();
	data.AABB_vbo.createData(nullptr, data.maxVertexCount * sizeof(AABBVertex), GL_DYNAMIC_DRAW);

	data.AABB_vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(AABBVertex), (void*)0);
	data.AABB_vao.addVertexBufferLayout(1, 3, GL_FLOAT, GL_FALSE, sizeof(AABBVertex), (void*)offsetof(AABBVertex, color));
	data.AABB_vao.unbind();
	data.AABB_vbo.unbind();
}

void Renderer::drawMesh(const std::vector<MeshVertex>& mesh)
{
	// Binding VAO, VBO, EBO
	data.Mesh_vao.bind();
	data.Mesh_vbo.bind();

	data.Mesh_vbo.updateData(mesh.data(), mesh.size() * sizeof(MeshVertex), 0);
	glDrawArrays(GL_TRIANGLES, 0, mesh.size());

	// Unbinding VAO, VBO
	data.Mesh_vao.unbind();
	data.Mesh_vbo.unbind();
}

//toto tu teoreticky mozem nechat, ale mozno to pomenovat inac napr drawBox, alebo nieco take
//a mala by ta metoda brat nejaky parameter nech ich dokaze 

void Renderer::drawBox(const std::vector<AABBVertex>& box)
{
	// Binding VAO, VBO, EBO
	data.AABB_vao.bind();
	data.AABB_vbo.bind();
	//data.AABB_ebo.bind();

	data.AABB_vbo.updateData(box.data(), box.size() * sizeof(AABBVertex), 0);

	//data.AABB_vbo.updateData(data.boundingBoxVertices.data(), data.boundingBoxVertices.size() * sizeof(AABBVertex), 0);

	// Draw all AABBs in a single call
	//glDrawElements(GL_LINES, data.maxIndexCount, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDrawArrays(GL_TRIANGLES, 0, data.boundingBoxVertices.size());
	glDrawArrays(GL_TRIANGLES, 0, box.size());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Unbinding VAO, VBO, EBO
	data.AABB_vao.unbind();
	data.AABB_vbo.unbind();
	data.AABB_ebo.unbind();
}

void Renderer::drawLines(std::vector<LineVertex>& lines)
{
	//Binding VAO,VBO
	data.Line_vao.bind();
	data.Line_vbo.bind();
	//Updade VBO
	data.Line_vbo.updateData(lines.data(), lines.size() * sizeof(LineVertex), 0);
	//Draw Mesh using vertexData
	glLineWidth(3.0f);
	glDrawArrays(GL_LINES, 0, lines.size());
	glLineWidth(0.5f);
	//Unbinding VAO,VBO
	data.Line_vao.unbind();
	data.Line_vbo.unbind();
}

void Renderer::drawPoints(std::vector<MeshPoint>& points)
{
	glDisable(GL_PROGRAM_POINT_SIZE);
	// Binding VAO, VBO, EBO
	data.Point_vao.bind();
	data.Point_vbo.bind();

	data.Mesh_vbo.updateData(points.data(), points.size() * sizeof(MeshPoint), 0);

	glPointSize(10.0f);

	glDrawArrays(GL_POINTS, 0, points.size());

	// Unbinding VAO, VBO
	data.Point_vao.unbind();
	data.Point_vbo.unbind();
}
