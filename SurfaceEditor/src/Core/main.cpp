#include <iostream>
#include <vector>
#include "../Core/Window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Core/Application.h"
#include "../Core/Input.h"
#include "../AABBBoundingRegion.h"
#include "Input.h"
#include "../Renderer/Renderer.h"
//#include "../Mesh.h"
#include "../Renderer/Framebuffer.h"
#include "../PolygonTraits.h"


import Renderer.Shader;
import Renderer.Buffers;
import DataStructures;
//import LayerSystem.Layer.ImGuiLayer;
import Scene;
//import Patterns.Observer;



float deltaTime = 0.0f;
float lastFrame = 0.0f;


Ray getCameraRay(const glm::vec3& cameraPosition, const glm::vec3& cameraFrontVector)
{
	return Ray(cameraPosition, cameraFrontVector);
}

#include "../DataStructures/HalfEdge.h"

//#include "../Patterns/Observer.h"
#include "../ViewPortsHolder.h"
#include "../Commands/AddPlaneCommand.h"
#include "../Callbacks/AddPlaneCallback.h"
//#include "../Commands/CommandRegistry.h"


#include "../Gui/AdditionLayer.h"
#include "../Gui/ToolBarLayer.h"
#include "../Callbacks/ToolBarLayerCallBack.h"
#include "../Commands/SelectMeshCommand.h"
#include "../Callbacks/SelectionCallBacks/SelectMeshCallBack.h"

#include "../Commands/DeselectMeshCommand.h"
#include "../Callbacks/DeselectMeshCallBack.h"

#include "../Commands/SelectFaceCommand.h"
#include "../Callbacks/SelectionCallBacks/SelectFaceCallBack.h"

#include "../Commands/SelectVertexCommand.h"
#include "../Callbacks/SelectionCallBacks/SelectVertexCallBack.h"

#include "../Commands/BasicSculptToolCommand.h"


//TRANSFORMLAYER
#include "../Gui/TransformLayer.h"
#include "../Callbacks/TransformLayerCallBacks/TransformAddPlaneCallBack.h"
#include "../Callbacks/TransformLayerCallBacks/TransformSelectMeshCallBack.h"

#include "../Commands/TransformMeshCommand.h"
#include "../Callbacks/TransformMeshCallBack.h"

//#include "../OBJExporter.h"
//#include "../OBJImporter.h"

//ImportExportLayer
#include "../Gui/ImportExportLayer.h"
#include "../Commands/ImportMeshesCommand.h"
#include "../Callbacks/ImportMeshesCallBack.h"

#include "../Commands/ExportMeshesCommand.h"
#include "../Callbacks/ExportMeshesCallBack.h"

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;


int main()
{
	Application& app = Application::getInstance(SCR_WIDTH, SCR_HEIGHT, "SurfaceEditor");
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	
	Ray ray = getCameraRay(camera.getState().position, camera.getState().frontVector);
	std::vector<LineVertex> rayLine;



	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader testShader = Shader("src\\Renderer\\quad.vert", "src\\Renderer\\fragColTest.frag");

	Shader pointsShader = Shader("src\\Renderer\\pointsShader.vert", "src\\Renderer\\pointsShader.frag");

	Shader linesShader = Shader("src\\Renderer\\linesShader.vert", "src\\Renderer\\linesShader.frag");

	//mesh shader
	Shader meshShader = Shader("src\\Renderer\\meshShader.vert", "src\\Renderer\\meshShader.frag", "src\\Renderer\\meshShader.geom");

	//normals shader
	Shader normalsShader = Shader("src\\Renderer\\normals.vert", "src\\Renderer\\normals.frag", "src\\Renderer\\normals.geom");


	Shader shaderSingleColor = Shader("src\\Renderer\\singleColor.vert", "src\\Renderer\\singleColor.frag");

	Shader shaderDilation = Shader("src\\Renderer\\quad.vert", "src\\Renderer\\dilate.frag");

	Shader shaderBlit = Shader("src\\Renderer\\quad.vert", "src\\Renderer\\blit.frag");

	Scene scene(25.0f, 25.0f, 25.0f);



	//access
	//scene.accessNeighbouringVertsAndFacesOfVertex(&mesh->m_halfEdgeMesh->m_vertices.at(0));


	//ToolBarLayer toolBarLayer(ToolBarLayer("ToolBarLayer"));
	//app.getLayerStack().addLayer(&toolBarLayer);
	

	//viewPortLayer
	ViewPortLayer* viewPortLayer = new ViewPortLayer("viewPortLayer");
	viewPortLayer->m_activeCamera = &camera;

	viewPortLayer->m_shaderSettings.m_faceShader = &meshShader;
	viewPortLayer->m_shaderSettings.m_meshShader = &meshShader;
	viewPortLayer->m_shaderSettings.m_normalShader = &normalsShader;
	viewPortLayer->m_shaderSettings.m_pointShader = &pointsShader;
	viewPortLayer->m_shaderSettings.m_edgeShader = &linesShader;
	app.getLayerStack().addLayer(viewPortLayer);

	ViewPortsHolder* viewPortsHolder = new ViewPortsHolder("viewPortsHolderLayer");
	viewPortsHolder->addViewPortLayer(viewPortLayer);
	viewPortsHolder->m_activeViewPortLayer = viewPortLayer;

	viewPortsHolder->m_scene = &scene;
	ViewPortsHolderContext::m_viewPortsHolder = viewPortsHolder;

	AddPlaneParams planeProperties;
	planeProperties.m_size = 50.0f;
	planeProperties.m_subdivisionLevel = 4;


	//ImporExportLayer
	ImportExportLayer* importExportLayer = new ImportExportLayer("importExportLayer");
	app.getLayerStack().addLayer(importExportLayer);

	//importMeshesCommand

	CommandRegistry::registerCommand<ImportMeshesCommand>();
	ImportMeshesCommand* importMeshesCommand = CommandRegistry::getCommand<ImportMeshesCommand>();

	ImportMeshesCallBack importMeshesCallBack;
	//viewPortHolder->observe(importMeshesCommand, &importMeshesCallBack);

	importMeshesCommand->addObserver(&importMeshesCallBack);
	importMeshesCallBack.observe(importMeshesCommand, &importMeshesCallBack);

	CommandRegistry::registerCommand<ExportMeshesCommand>();
	ExportMeshesCommand* exportMeshesCommand = CommandRegistry::getCommand<ExportMeshesCommand>();

	ExportMeshesCallBack exportMeshesCallBack;
	//viewPortHolder->observe(exportMeshesCommand, &exportMeshesCallBack);

	exportMeshesCommand->addObserver(&exportMeshesCallBack);
	exportMeshesCallBack.observe(exportMeshesCommand, &exportMeshesCallBack);

	CommandRegistry::registerCommand<AddPlaneCommand>();
	AddPlaneCommand* addPlaneCommand = CommandRegistry::getCommand<AddPlaneCommand>();
	
	AddPlaneCallback addPlaneCallBack;
	//viewPortHolder->observe(addPlaneCommand, &addPlaneCallBack);

	addPlaneCommand->addObserver(&addPlaneCallBack);
	addPlaneCallBack.observe(addPlaneCommand, &addPlaneCallBack);

	SelectMeshCallBack selectMeshCallBack;
	CommandRegistry::registerCommand<SelectMeshCommand>();
	SelectMeshCommand* selectMeshCommand = CommandRegistry::getCommand<SelectMeshCommand>();

	selectMeshCommand->addObserver(&selectMeshCallBack);
	selectMeshCallBack.observe(selectMeshCommand, &selectMeshCallBack);
	//viewPortHolder->observe(selectMeshCommand, &selectMeshCallBack);

	
	DeselectMeshCallBack deselectMeshCallBack;
	CommandRegistry::registerCommand<DeselectMeshCommand>();
	DeselectMeshCommand* deselectMeshCommand = CommandRegistry::getCommand<DeselectMeshCommand>();
	viewPortsHolder->observe(deselectMeshCommand, &deselectMeshCallBack);
	
	SelectFaceCallBack selectFaceCallBack;
	CommandRegistry::registerCommand<SelectFaceCommand>();
	SelectFaceCommand* selectFaceCommand = CommandRegistry::getCommand<SelectFaceCommand>();
	viewPortsHolder->observe(selectFaceCommand, &selectFaceCallBack);

	SelectVertexCallBack selectVertexCallBack;
	CommandRegistry::registerCommand<SelectVertexCommand>();
	SelectVertexCommand* selectVertexCommand = CommandRegistry::getCommand<SelectVertexCommand>();
	viewPortsHolder->observe(selectVertexCommand, &selectVertexCallBack);

	ToolBarLayer toolBarLayer("ToolBarLayer");
	app.getLayerStack().addLayer(&toolBarLayer);

	ToolBarLayerCallBack toolBarLayerCallBack;
	//viewPortHolder->observe(&toolBarLayer, &toolBarLayerCallBack);

	toolBarLayer.addObserver(&toolBarLayerCallBack);
	toolBarLayerCallBack.observe(&toolBarLayer, &toolBarLayerCallBack);

	//toolBarLayerCallBack.observe(&toolBarLayer, &toolBarLayerCallBack);


	//Transform Layer
	//TransformLayer transformLayer(viewPortHolder, "TransformLayer");

	//TransformAddPlaneCallBack transformAddPlaneCallBack(&transformLayer, &addPlaneCallBack);

	//selectMeshCallBack.addObserver(&transformLayer);
	//TransformSelectMeshCallBack transformSelectMeshCallBack(&transformLayer, &selectMeshCallBack);
	//transformLayer.observe(&selectMeshCallBack, &transformSelectMeshCallBack);
	//addPlaneCallBack.addObserver(&transformLayer);
	//transformLayer.observe(&addPlaneCallBack, &transformAddPlaneCallBack);


	CommandRegistry::registerCommand<TransformMeshCommand>();
	TransformMeshCommand* transformMeshCommand = CommandRegistry::getCommand<TransformMeshCommand>();
	TransformMeshCallBack transformMeshCallBack;
	viewPortsHolder->observe(transformMeshCommand, &transformMeshCallBack);

	

	//SCULPT TOOL
	BasicSculptToolCommand basicSculptToolCommand;


	//AdditionLayer
	AdditionLayer additionLayer("AdditionLayer");
	app.getLayerStack().addLayer(&additionLayer);

	//TransformLayer
	//app.getLayerStack().addLayer(&transformLayer);

	scene.m_rendererData.aabbData.clearAABBData();

	for (auto& entry : scene.m_meshFaceOctreesMap) {
		const glm::vec3& key = entry.first;
		Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>& octree = entry.second;
		for (auto& octreeNode : octree) {
			octreeNode.getBounds();
			scene.m_rendererData.aabbData.collectAABBData(octreeNode.getBounds());
		}
	}

	//toolBarLayer.addObserver(&viewPortLayer);
	glm::mat4 model = glm::mat4(1.0f);


	//mesh shader
	meshShader.bind();
	meshShader.setMat4("u_model", model);
	meshShader.unbind();

	//normals shader
	normalsShader.bind();
	normalsShader.setMat4("u_model", model);
	normalsShader.unbind();

	//SingleColor shader
	shaderSingleColor.bind();
	shaderSingleColor.setMat4("u_model", model);
	shaderSingleColor.unbind();

	//points shader
	pointsShader.bind();
	pointsShader.setMat4("u_model", model);
	pointsShader.unbind();

	//lines shader
	linesShader.bind();
	linesShader.setMat4("u_model", model);
	linesShader.unbind();


	while (!glfwWindowShouldClose(app.getWindow().getWindowHandle()))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//app.getWindow().processInput(camera, deltaTime);
		//Input::updateMousePosition();


		linesShader.bind();
		glm::mat4 projection = glm::mat4(1.0f);

		float nearPlane = 1.0f;
		float farPlane = 1000.0f;
		projection = glm::perspective(glm::radians(camera.getState().zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), nearPlane, farPlane);
		linesShader.setMat4("u_projection", projection);


		glm::mat4 view = camera.getState().lookAtMatrix;
		linesShader.setMat4("u_view", view);
		
		//linesShader.setMat4("u_model", model);

		linesShader.unbind();


		//mesh shader
		meshShader.bind();
		meshShader.setMat4("u_projection", projection);
		meshShader.setMat4("u_view", view);
		//meshShader.setMat4("u_model", model);
		meshShader.unbind();

		//normals shader
		normalsShader.bind();
		normalsShader.setMat4("u_projection", projection);
		normalsShader.setMat4("u_view", view);
		//normalsShader.setMat4("u_model", model);
		normalsShader.unbind();

		//SingleColor shader
		shaderSingleColor.bind();
		shaderSingleColor.setMat4("u_projection", projection);
		shaderSingleColor.setMat4("u_view", view);
		//shaderSingleColor.setMat4("u_model", model);
		shaderSingleColor.unbind();

		//points shader
		pointsShader.bind();
		pointsShader.setMat4("u_projection", projection);
		pointsShader.setMat4("u_view", view);
		//pointsShader.setMat4("u_model", model);
		pointsShader.unbind();

		ray = Ray::fromMousePos(camera, projection, view, app.getWindow());

		if (Input::isKeyDown(GLFW_KEY_C))
		{
			basicSculptToolCommand.applyBrush(ray, 20.0f, 5.0f);

		}

		if (Input::isKeyDown(GLFW_KEY_V))
		{
			basicSculptToolCommand.applyBrush(ray, 20.0f, -5.0f);

		}

		if(Input::isKeyDown(GLFW_KEY_T))
		{
			//viewPortLayer.deleteSelectedMeshes();
			//delete them from renderer aswell!
			scene.m_rendererData.aabbData.clearAABBData();

			for (auto& entry : scene.m_meshFaceOctreesMap) {
				const glm::vec3& key = entry.first;
				Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>& octree = entry.second;
				for (auto& octreeNode : octree) {
					octreeNode.getBounds();
					scene.m_rendererData.aabbData.collectAABBData(octreeNode.getBounds());
				}
			}

		}


		glViewport(0, 0, ViewPortsHolderContext::m_viewPortsHolder->m_activeViewPortLayer->m_screenSettings.SCR_WIDTH,
			ViewPortsHolderContext::m_viewPortsHolder->m_activeViewPortLayer->m_screenSettings.SCR_HEIGHT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		for (auto& [mesh, faceMap] : viewPortsHolder->m_scene->m_meshesFaceOctreeMap) {

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//chcem zapisovat do depth bufferu
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LESS);

			//nechcem zapisovat do stencil bufferu
			glStencilMask(0x00);
			glDisable(GL_STENCIL_TEST);

			//mesh shader
			ViewPortsHolderContext::m_viewPortsHolder->m_activeViewPortLayer->m_shaderSettings.m_faceShader->bind();
			Renderer::drawMesh(mesh->m_combinedVertexDataMatVector.m_vertexData);
			ViewPortsHolderContext::m_viewPortsHolder->m_activeViewPortLayer->m_shaderSettings.m_faceShader->unbind();

			//for normals
			ViewPortsHolderContext::m_viewPortsHolder->m_activeViewPortLayer->m_shaderSettings.m_normalShader->bind();
			Renderer::drawMesh(mesh->m_combinedVertexDataMatVector.m_vertexData);
			ViewPortsHolderContext::m_viewPortsHolder->m_activeViewPortLayer->m_shaderSettings.m_normalShader->unbind();

			/*
			//points
			ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_pointShader->bind();
			Renderer::drawPoints(std::get<2>(tuple).m_points);
			ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_pointShader->unbind();

			//lines
			ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_edgeShader->bind();
			Renderer::drawBoundingBoxes();
			Renderer::drawLines(std::get<2>(tuple).m_edges);
			ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_edgeShader->unbind();
			*/

		}
		ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_edgeShader->bind();
		for (const auto& [region, vertices] : scene.m_rendererData.aabbData.vaoDataMap) {
			
			Renderer::drawBox(vertices);
		}
		ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_edgeShader->unbind();

		app.run();

		
		app.getWindow().update();
	}

	app.close();
	return 0;

}