#pragma once
#include <vector>
#include "Patterns/Observer.h"
#include "Patterns/Command.h"
#include <glm/mat4x4.hpp>
#include "../Renderer/VertexDataStructs.h"
import LayerSystem.Layer.ViewPortLayer;
import LayerSystem.Layer;


class ViewPortHolder : public Layer, public Observer
{
public:

	
	struct MeshRenderingFlags
	{
		bool RENDER_NORMALS = true;
		bool RENDER_POINTS = true;
		bool RENDER_EDGES = true;
		bool RENDER_FACES = true;
	};

	struct MeshRenderingShaderData
	{
		MeshRenderingShaderData(Shader point_shader, Shader edge_shader, Shader normal_shader,
			const Shader face_shader)
			: PointShader(point_shader),
			EdgeShader(edge_shader),
			NormalShader(normal_shader),
			FaceShader(face_shader)
		{
		}

		Shader PointShader;
		Shader EdgeShader;
		Shader NormalShader;
		Shader FaceShader;
	};

	struct MeshRenderingVAOData
	{
	public:
		std::vector<LineVertex> m_edges;
		std::vector<MeshPoint> m_points;
	};

	std::map<Mesh*, std::tuple<MeshRenderingFlags, MeshRenderingShaderData, MeshRenderingVAOData>> m_meshesShaderData;
	
public:

	ViewPortHolder(ViewPortLayer* viewPortLayer): Layer("ViewPortHolder")
	{
		m_viewPortLayer = viewPortLayer;
	}


	void renderMeshes()
	{
		glViewport(0, 0, m_viewPortLayer->SCR_WIDTH, m_viewPortLayer->SCR_HEIGHT);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		
		
		for (auto& [mesh, tuple] : m_meshesShaderData)
		{

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//chcem zapisovat do depth bufferu
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LESS);

			//nechcem zapisovat do stencil bufferu
			glStencilMask(0x00);
			glDisable(GL_STENCIL_TEST);

			//mesh shader
			std::get<1>(tuple).FaceShader.bind();
			Renderer::drawMeshDefault(mesh->m_combinedVertexDataMatVector.m_vertexData);
			std::get<1>(tuple).FaceShader.unbind();

			//for normals
			std::get<1>(tuple).NormalShader.bind();
			Renderer::drawMeshDefault(mesh->m_combinedVertexDataMatVector.m_vertexData);
			std::get<1>(tuple).NormalShader.unbind();

			//points
			std::get<1>(tuple).PointShader.bind();
			Renderer::drawPoints(std::get<2>(tuple).m_points);
			std::get<1>(tuple).PointShader.unbind();

			//lines
			std::get<1>(tuple).EdgeShader.bind();
			Renderer::drawBoundingBoxes();
			Renderer::drawLines(std::get<2>(tuple).m_edges);
			std::get<1>(tuple).EdgeShader.unbind();

			/*
			auto it = m_selectedMeshes.find(mesh);

			

			// Check if the meshToFind was found in the set
			if (it != m_selectedMeshes.end()) {

				// 1st. render pass, draw highlighted objects
	// --------------------------------------------------------------------

	// render to target A
				glBindFramebuffer(GL_FRAMEBUFFER, m_viewPortLayer->m_targetA.fbo);
				glViewport(0, 0, m_viewPortLayer->SCR_WIDTH, m_viewPortLayer->SCR_HEIGHT);
				glClearColor(1.f, 1.f, 1.f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



				m_viewPortLayer->m_shaderSingleColor->bind();
				
				Renderer::drawMeshDefault(mesh->m_combinedVertexDataMatVector.m_vertexData);

				// dilate contour and render to target B
				glBindFramebuffer(GL_FRAMEBUFFER, m_viewPortLayer->m_targetB.fbo);
				glViewport(0, 0, m_viewPortLayer->SCR_WIDTH, m_viewPortLayer->SCR_HEIGHT);
				glStencilMask(0xFF);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
				m_viewPortLayer->m_shaderDilation->bind();
				m_viewPortLayer->m_shaderDilation->setFloat("radius", 5.F);
				m_viewPortLayer->m_shaderDilation->setFloat("gridX", 1.F / (float)m_viewPortLayer->SCR_WIDTH);
				m_viewPortLayer->m_shaderDilation->setFloat("gridY", 1.F / (float)m_viewPortLayer->SCR_HEIGHT);
				m_viewPortLayer->m_shaderDilation->setInt("screenTexture", 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_viewPortLayer->m_targetA.colorTexId);
				glBindVertexArray(RendererData::screenQuadVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glStencilFunc(GL_ALWAYS, 0, 0xFF);

				// copy stencil buffer from target B
				glBindFramebuffer(GL_READ_FRAMEBUFFER, m_viewPortLayer->m_targetB.fbo);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
				glBlitFramebuffer(0, 0, m_viewPortLayer->SCR_WIDTH, m_viewPortLayer->SCR_HEIGHT, 0, 0, m_viewPortLayer->SCR_WIDTH, m_viewPortLayer->SCR_HEIGHT,
					GL_STENCIL_BUFFER_BIT, GL_NEAREST);

				// bind to main frame buffer
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				// blit target B to the default frame buffer
				glStencilFunc(GL_EQUAL, 1, 0xFF);
				glStencilMask(0x00);
				glDisable(GL_DEPTH_TEST);
				m_viewPortLayer->m_shaderBlit->bind();
				m_viewPortLayer->m_shaderBlit->setInt("screenTexture", 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_viewPortLayer->m_targetB.colorTexId);
				glBindVertexArray(RendererData::screenQuadVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				// disable stencil buffer
				glStencilMask(0xFF);
				glStencilFunc(GL_ALWAYS, 0, 0xFF);
				glEnable(GL_DEPTH_TEST);


				// 2nd. render pass: draw objects normally
	// -----------------------------------------------------------------------------------------------------------------------------


				//mesh shader
				std::get<1>(tuple).FaceShader.bind();
				Renderer::drawMeshDefault(mesh->m_combinedVertexDataMatVector.m_vertexData);
				std::get<1>(tuple).FaceShader.unbind();

				//for normals
				std::get<1>(tuple).NormalShader.bind();
				Renderer::drawMeshDefault(mesh->m_combinedVertexDataMatVector.m_vertexData);
				std::get<1>(tuple).NormalShader.unbind();


				//points
				std::get<1>(tuple).PointShader.bind();
				Renderer::drawPoints(std::get<2>(tuple).m_points);
				std::get<1>(tuple).PointShader.unbind();

				//lines
				std::get<1>(tuple).EdgeShader.bind();
				Renderer::drawBoundingBoxes();
				Renderer::drawLines(std::get<2>(tuple).m_edges);
				std::get<1>(tuple).EdgeShader.unbind();

			}
			else {
				// Mesh* was not found in the set

				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				//chcem zapisovat do depth bufferu
				glEnable(GL_DEPTH_TEST);
				glDepthMask(GL_TRUE);
				glDepthFunc(GL_LESS);

				//nechcem zapisovat do stencil bufferu
				glStencilMask(0x00);
				glDisable(GL_STENCIL_TEST);

				//mesh shader
				std::get<1>(tuple).FaceShader.bind();
				Renderer::drawMeshDefault(mesh->m_combinedVertexDataMatVector.m_vertexData);
				std::get<1>(tuple).FaceShader.unbind();
			}

			*/

			/*
			if (std::get<0>(tuple).RENDER_FACES)
			{
				std::get<1>(tuple).FaceShader->bind();
				Renderer::drawMeshDefault(mesh->m_combinedVertexDataMatVector.m_vertexData);
				std::get<1>(tuple).FaceShader->unbind();
			}

			if (std::get<0>(tuple).RENDER_NORMALS)
			{
				std::get<1>(tuple).NormalShader->bind();
				Renderer::drawMeshDefault(mesh->m_combinedVertexDataMatVector.m_vertexData);
				std::get<1>(tuple).NormalShader->unbind();
			}

			if (std::get<0>(tuple).RENDER_POINTS)
			{
				std::get<1>(tuple).PointShader->bind();
				Renderer::drawPoints(std::get<2>(tuple).m_points);
				std::get<1>(tuple).PointShader->unbind();
			}

			if (std::get<0>(tuple).RENDER_EDGES)
			{
				std::get<1>(tuple).EdgeShader->bind();
				Renderer::drawLines(std::get<2>(tuple).m_edges);
				//Renderer::drawMeshDefault(mesh->m_combinedVertexDataMatVector.m_vertexData);
				std::get<1>(tuple).EdgeShader->unbind();
			}
			*/

		}
		
	}


	virtual void onEvent(Event& event) override
	{
		//tuto budu prichadzat eventy a ja ich potom nasledne budem moct posielat do viewPortlayeru podla toho co som stlacil,
		//cize budem 
	}

	void addCommandToQueue(Command* command)
	{
		m_commandsQueue.push_back(command);
	}


	//unhide later
	void render()
	{

		// render
	// ------
		glViewport(0, 0, m_viewPortLayer->SCR_WIDTH, m_viewPortLayer->SCR_HEIGHT);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glStencilMask(0xFF);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);




		for (const auto& pair : m_viewPortLayer->getScene()->m_meshesFaceOctreeMap) {
			//need to know if the mesh is selected

			auto it = m_selectedMeshes.find(pair.first);


			// Check if the meshToFind was found in the set
			if (it != m_selectedMeshes.end()) {



				// Mesh* was found in the set


				// 1st. render pass, draw highlighted objects
	// --------------------------------------------------------------------

	// render to target A
				glBindFramebuffer(GL_FRAMEBUFFER, m_viewPortLayer->m_targetA.fbo);
				glViewport(0, 0, m_viewPortLayer->SCR_WIDTH, m_viewPortLayer->SCR_HEIGHT);
				glClearColor(0.f, 0.f, 0.f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



				m_viewPortLayer->m_shaderSingleColor->bind();
				//shaderSingleColor.setMat4("u_projection", projection);
				///shaderSingleColor.setMat4("u_view", view);
				//shaderSingleColor.setMat4("u_model", model);
				Renderer::drawMeshDefault(pair.first->m_combinedVertexDataMatVector.m_vertexData);

				// dilate contour and render to target B
				glBindFramebuffer(GL_FRAMEBUFFER, m_viewPortLayer->m_targetB.fbo);
				glViewport(0, 0, m_viewPortLayer->SCR_WIDTH, m_viewPortLayer->SCR_HEIGHT);
				glStencilMask(0xFF);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
				m_viewPortLayer->m_shaderDilation->bind();
				m_viewPortLayer->m_shaderDilation->setFloat("radius", 5.F);
				m_viewPortLayer->m_shaderDilation->setFloat("gridX", 1.F / (float)m_viewPortLayer->SCR_WIDTH);
				m_viewPortLayer->m_shaderDilation->setFloat("gridY", 1.F / (float)m_viewPortLayer->SCR_HEIGHT);
				m_viewPortLayer->m_shaderDilation->setInt("screenTexture", 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_viewPortLayer->m_targetA.colorTexId);
				glBindVertexArray(RendererData::screenQuadVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glStencilFunc(GL_ALWAYS, 0, 0xFF);

				// copy stencil buffer from target B
				glBindFramebuffer(GL_READ_FRAMEBUFFER, m_viewPortLayer->m_targetB.fbo);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
				glBlitFramebuffer(0, 0, m_viewPortLayer->SCR_WIDTH, m_viewPortLayer->SCR_HEIGHT, 0, 0, m_viewPortLayer->SCR_WIDTH, m_viewPortLayer->SCR_HEIGHT,
					GL_STENCIL_BUFFER_BIT, GL_NEAREST);

				// bind to main frame buffer
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				// blit target B to the default frame buffer
				glStencilFunc(GL_EQUAL, 1, 0xFF);
				glStencilMask(0x00);
				glDisable(GL_DEPTH_TEST);
				m_viewPortLayer->m_shaderBlit->bind();
				m_viewPortLayer->m_shaderBlit->setInt("screenTexture", 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_viewPortLayer->m_targetB.colorTexId);
				glBindVertexArray(RendererData::screenQuadVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				// disable stencil buffer
				glStencilMask(0xFF);
				glStencilFunc(GL_ALWAYS, 0, 0xFF);
				glEnable(GL_DEPTH_TEST);


				// 2nd. render pass: draw objects normally
	// -----------------------------------------------------------------------------------------------------------------------------


				//mesh shader
				m_viewPortLayer->m_meshShader->bind();
				//meshShader.setMat4("u_projection", projection);
				//meshShader.setMat4("u_view", view);
				//meshShader.setMat4("u_model", model);

				Renderer::drawMeshDefault(pair.first->m_combinedVertexDataMatVector.m_vertexData);


				//for normals
				m_viewPortLayer->m_normalsShader->bind();
				Renderer::drawMeshDefault(pair.first->m_combinedVertexDataMatVector.m_vertexData);
				m_viewPortLayer->m_normalsShader->unbind();


				/*
				//points
				m_viewPortLayer->m_pointsShader->bind();
				Renderer::drawPoints(pair.first->m_meshRenderingData.m_points);
				m_viewPortLayer->m_pointsShader->unbind();


				m_viewPortLayer->m_linesShader->bind();
				Renderer::drawBoundingBoxes();
				//Renderer::drawLines(rayLine);


				Renderer::drawLines(pair.first->m_meshRenderingData.m_edges);


				m_viewPortLayer->m_linesShader->unbind();
				*/


			}
			else {
				// Mesh* was not found in the set

				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				//chcem zapisovat do depth bufferu
				glEnable(GL_DEPTH_TEST);
				glDepthMask(GL_TRUE);
				glDepthFunc(GL_LESS);

				//nechcem zapisovat do stencil bufferu
				glStencilMask(0x00);
				glDisable(GL_STENCIL_TEST);

				//glStencilFunc(GL_ALWAYS, 0, 0xFF);
				//for mesh
				m_viewPortLayer->m_meshShader->bind();
				Renderer::drawMeshDefault(pair.first->m_combinedVertexDataMatVector.m_vertexData);
				m_viewPortLayer->m_meshShader->unbind();

			}
		}

	}


public:
	//selectedMeshes
	std::set<Mesh*> m_selectedMeshes;
	//selectedFaces
	//nepouzivane pri renderovani
	std::map<Mesh*, std::set<HalfEdgeDS::Face*>> m_selectedFaces;
	std::map<Mesh*, std::set<HalfEdgeDS::Vertex*>> m_selectedVertices;

	std::vector<Command*> m_commandsQueue;
	ViewPortLayer* m_viewPortLayer;

	//temporary
	size_t m_currentMeshId = 0;
};