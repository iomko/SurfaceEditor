#define NOMINMAX  // Prevents Windows.h from defining min/max macros


#include <limits>
#include <Windows.h>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Input.h"
#include "../Primitives/AABB.h"
#include "../Core/Application.h"
#include "../Core/Input.h"
#include "../Core/Window.h"
#include "../Renderer/Renderer.h"
//#include "../Mesh.h"
#include "../Callables/FunctionComposer.h"
#include "../Callbacks/BrushToolCallBack.h"
#include "../UI/SculptToolsLayer.h"
#include "../Scene/Mesh.h"


import Renderer.Shader;
#include "../Renderer/Buffers.h"
#include "../DataStructures/Octree.h"
#include "tiff/tiffio.h"
//import LayerSystem.Layer.ImGuiLayer;
//import Patterns.Observer;



float deltaTime = 0.0f;
float lastFrame = 0.0f;


#include "../DataStructures/HalfEdge.h"

//#include "../Patterns/Observer.h"
#include "../ViewPortsController.h"
#include "../Callbacks/AddPlaneCallback.h"
#include "../Commands/AddPlaneCommand.h"
#include "../Tools/ToolRegistry.h"
//#include "../Commands/CommandRegistry.h"


#include "../Callbacks/DeselectFaceCallBack.h"
#include "../Callbacks/SelectMeshCallBack.h"
#include "../Callbacks/SelectionLayerCallBack.h"
#include "../Commands/SelectMeshCommand.h"
#include "../UI/AdditionLayer.h"
#include "../UI/RemovalLayer.h"
#include "../UI/SelectionLayer.h"

#include "../Callbacks/SelectFaceCallBack.h"

#include "../Callbacks/DeselectMeshCallBack.h"
#include "../Commands/DeselectMeshCommand.h"

#include "../Commands/DeselectFaceCommand.h"
#include "../Commands/SelectFaceCommand.h"

#include "../Callbacks/DeleteSelectedFacesCallBack.h"
#include "../Commands/DeleteSelectedFacesCommand.h"

#include "../Callbacks/DeleteSelectedMeshesCallBack.h"
#include "../Commands/DeleteSelectedMeshesCommand.h"

#include "../Commands/BasicSculptToolCommand.h"

#include "../Commands/BrushToolCommand.h"
#include "../Tools/BrushTool.h"
#include "../Tools/FaceSelectionTool.h"

#include "../Callables/PlaneVertexGenCallable.h"
#include "../Callables/MeshVaoInitCallable.h"
#include "../Callables/SceneMeshAdderCallable.h"

//ImportExportLayer
#include "../Callbacks/ImportMeshesCallback.h"
#include "../Commands/ImportMeshesCommand.h"
#include "../UI/ImportExportLayer.h"

#include "../Callbacks/ExportMeshesCallback.h"
#include "../Commands/ExportMeshesCommand.h"

#include "../Callables/FetchedSurfaceVertexGenCallable.h"
#include "../Callbacks/FetchSurfaceCallBack.h"

//INTERACTION_HANDLER
#include "../Tools/InteractionHandler.h"
#include "../Tools/Tool.h"

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;



//=====TESTING THE FUNCTION_COMPOSER=====


//=====PLANE_MESH_DATA=====

//OUTPUT


void clearAABBData()
{
	Renderer::s_stageData.aabbVertsMap.clear();
}

void collectAABBData(const AABBBoundingRegion& aabb) {
	glm::vec3 vertexColor = glm::vec3(0.0f, 1.0f, 0.0f);
	//left side
	glm::vec3 bottomUpLeft = aabb.getMinBoundsPos();
	glm::vec3 bottomDownLeft = glm::vec3(aabb.getMinBoundsPos().x, aabb.getMinBoundsPos().y, aabb.getMaxBoundsPos().z);
	glm::vec3 upperDownLeft = glm::vec3(aabb.getMinBoundsPos().x, aabb.getMaxBoundsPos().y, aabb.getMaxBoundsPos().z);
	glm::vec3 upperUpLeft = glm::vec3(aabb.getMinBoundsPos().x, aabb.getMaxBoundsPos().y, aabb.getMinBoundsPos().z);
	//right side
	glm::vec3 bottomUpRight = glm::vec3(aabb.getMaxBoundsPos().x, aabb.getMinBoundsPos().y, aabb.getMinBoundsPos().z);
	glm::vec3 bottomDownRight = glm::vec3(aabb.getMaxBoundsPos().x, aabb.getMinBoundsPos().y, aabb.getMaxBoundsPos().z);
	glm::vec3 upperDownRight = aabb.getMaxBoundsPos();
	glm::vec3 upperUpRight = glm::vec3(aabb.getMaxBoundsPos().x, aabb.getMaxBoundsPos().y, aabb.getMinBoundsPos().z);

	RendererStageData::AABBVertex AABB_vertices[] = {
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

	Renderer::s_stageData.aabbVertsMap[aabb].insert(
		Renderer::s_stageData.aabbVertsMap[aabb].end(),
		std::begin(AABB_vertices),
		std::end(AABB_vertices)
	);
}

int main()
{
	//--INITIALIZATIONS_OF_FUNCTION_COMPOSERS--

	//--ADD_PLANE_COMPOSER--

	FunctionComposer addPlaneComposer;
	FunctionNode* addPlaneRoot = addPlaneComposer.initRoot<PlaneVertexGenCallable>();
	addPlaneComposer.addFunc<MeshVaoInitCallable>(addPlaneRoot);
	addPlaneComposer.addFunc<SceneMeshAdderCallable>(addPlaneRoot);
	AddPlaneCallback addPlaneCallBack(addPlaneComposer);

	//---FETCH_SURFACE_COMPOSER---
	FunctionComposer fetchSurfaceComposer;
	FunctionNode* fetchSurfaceRoot = fetchSurfaceComposer.initRoot<FetchedSurfaceVertexGenCallable>();
	fetchSurfaceComposer.addFunc<MeshVaoInitCallable>(fetchSurfaceRoot);
	fetchSurfaceComposer.addFunc<SceneMeshAdderCallable>(fetchSurfaceRoot);
	FetchSurfaceCallBack fetchSurfaceCallBack(fetchSurfaceComposer);


	Application& app = Application::getInstance(SCR_WIDTH, SCR_HEIGHT, "SurfaceEditor");
	Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	Ray ray = Ray::getCameraRay(*camera);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader linesShader = Shader("src\\Renderer\\Shaders\\linesShader.vert", "src\\Renderer\\Shaders\\linesShader.frag");

	//mesh shader
	Shader meshShader = Shader("src\\Renderer\\Shaders\\meshShader.vert", "src\\Renderer\\Shaders\\meshShader.frag");

	//normals shader
	//Shader normalsShader = Shader("src\\Renderer\\normals.vert", "src\\Renderer\\normals.frag", "src\\Renderer\\normals.geom");


	Scene scene(25.0f, 25.0f, 25.0f);


	//viewPortLayer
	ViewPortLayer* viewPortLayer = new ViewPortLayer("viewPortLayer");
	viewPortLayer->m_camera = camera;

	viewPortLayer->m_shaderSettings.m_faceShader = &meshShader;
	viewPortLayer->m_shaderSettings.m_meshShader = &meshShader;
	viewPortLayer->m_shaderSettings.m_edgeShader = &linesShader;

	ViewPortsController* viewPortsHolder = new ViewPortsController();
	viewPortsHolder->addLayer(viewPortLayer);
	viewPortsHolder->m_activeViewPortLayer = viewPortLayer;
	SelectionController* selectionController = new SelectionController();

	viewPortsHolder->m_scene = &scene;
	ViewPortsHolderContext::s_viewPortsHolder = viewPortsHolder;
	ViewPortsHolderContext::s_selectionController = selectionController;
	ViewPortsHolderContext::s_camera = camera;
	ViewPortsHolderContext::s_window = app.m_window;


	PlaneParams planeProperties;
	planeProperties.m_size = 50.0f;
	planeProperties.m_subdivisionLevel = 4;

	//ViewPortLayer
	app.getLayerStack().addLayer(viewPortLayer);

	//ImporExportLayer
	ImportExportLayer* importExportLayer = new ImportExportLayer("importExportLayer");
	app.getLayerStack().addLayer(importExportLayer);

	//importMeshesCommand

	CommandRegistry::registerCommand<ImportMeshesCommand>();
	ImportMeshesCommand* importMeshesCommand = CommandRegistry::getCommand<ImportMeshesCommand>();

	ImportMeshesCallback importMeshesCallback;
	//viewPortHolder->observe(importMeshesCommand, &importMeshesCallBack);

	importMeshesCommand->addObserver(&importMeshesCallback);

	//importMeshesCommand->addObserver(&importMeshesCallBack);
	importMeshesCallback.observe(importMeshesCommand, &importMeshesCallback);
	//importMeshesCallBack.observe(importMeshesCommand, &importMeshesCallBack);

	CommandRegistry::registerCommand<ExportMeshesCommand>();
	ExportMeshesCommand* exportMeshesCommand = CommandRegistry::getCommand<ExportMeshesCommand>();

	ExportMeshesCallback exportMeshesCallback;
	//viewPortHolder->observe(exportMeshesCommand, &exportMeshesCallBack);

	exportMeshesCommand->addObserver(&exportMeshesCallback);
	exportMeshesCallback.observe(exportMeshesCommand, &exportMeshesCallback);

	CommandRegistry::registerCommand<AddPlaneCommand>();
	AddPlaneCommand* addPlaneCommand = CommandRegistry::getCommand<AddPlaneCommand>();

	addPlaneCommand->addObserver(&addPlaneCallBack);
	addPlaneCallBack.observe(addPlaneCommand, &addPlaneCallBack);

	CommandRegistry::registerCommand<FetchSurfaceCommand>();
	FetchSurfaceCommand* fetchSurfaceCommand = CommandRegistry::getCommand<FetchSurfaceCommand>();

	fetchSurfaceCommand->addObserver(&fetchSurfaceCallBack);
	fetchSurfaceCallBack.observe(fetchSurfaceCommand, &fetchSurfaceCallBack);

	SelectMeshCallBack selectMeshCallBack;
	CommandRegistry::registerCommand<SelectMeshCommand>();
	SelectMeshCommand* selectMeshCommand = CommandRegistry::getCommand<SelectMeshCommand>();

	ToolRegistry::registerTool<MeshSelectionTool>(selectMeshCommand, new SelectionHandler<SelectMeshCommand>());

	selectMeshCommand->addObserver(&selectMeshCallBack);
	selectMeshCallBack.observe(selectMeshCommand, &selectMeshCallBack);

	BrushToolCallBack brushToolCallBack;
	CommandRegistry::registerCommand<BrushToolCommand>();
	BrushToolCommand* brushToolCommand = CommandRegistry::getCommand<BrushToolCommand>();

	brushToolCommand->addObserver(&brushToolCallBack);
	brushToolCallBack.observe(brushToolCommand, &brushToolCallBack);

	ToolRegistry::registerTool<BrushTool>(brushToolCommand, new BrushInteractionHandler());

	DeselectMeshCallBack deselectMeshCallBack;
	CommandRegistry::registerCommand<DeselectMeshCommand>();
	DeselectMeshCommand* deselectMeshCommand = CommandRegistry::getCommand<DeselectMeshCommand>();

	deselectMeshCommand->addObserver(&deselectMeshCallBack);
	deselectMeshCallBack.observe(deselectMeshCommand, &deselectMeshCallBack);

	SelectFaceCallBack selectFaceCallBack;
	CommandRegistry::registerCommand<SelectFaceCommand>();
	SelectFaceCommand* selectFaceCommand = CommandRegistry::getCommand<SelectFaceCommand>();
	selectFaceCommand->addObserver(&selectFaceCallBack);
	selectFaceCallBack.observe(selectFaceCommand, &selectFaceCallBack);

	ToolRegistry::registerTool<FaceSelectionTool>(selectFaceCommand, new SelectionHandler<SelectFaceCommand>());

	DeselectFaceCallBack deselectFaceCallBack;
	CommandRegistry::registerCommand<DeselectFaceCommand>();
	DeselectFaceCommand* deselectFaceCommand = CommandRegistry::getCommand<DeselectFaceCommand>();
	deselectFaceCommand->addObserver(&deselectFaceCallBack);
	deselectFaceCallBack.observe(deselectFaceCommand, &deselectFaceCallBack);

	DeleteSelectedFacesCallBack deleteSelectedFacesCallBack;
	CommandRegistry::registerCommand<DeleteSelectedFacesCommand>();
	DeleteSelectedFacesCommand* deleteSelectedFacesCommand = CommandRegistry::getCommand<DeleteSelectedFacesCommand>();
	deleteSelectedFacesCommand->addObserver(&deleteSelectedFacesCallBack);
	deleteSelectedFacesCallBack.observe(deleteSelectedFacesCommand, &deleteSelectedFacesCallBack);

	DeleteSelectedMeshesCallBack deleteSelectedMeshesCallBack;
	CommandRegistry::registerCommand<DeleteSelectedMeshesCommand>();
	DeleteSelectedMeshesCommand* deleteSelectedMeshesCommand = CommandRegistry::getCommand<DeleteSelectedMeshesCommand>();
	deleteSelectedMeshesCommand->addObserver(&deleteSelectedMeshesCallBack);
	deleteSelectedMeshesCallBack.observe(deleteSelectedMeshesCommand, &deleteSelectedMeshesCallBack);

	SelectionLayer selectionLayer("SelectionLayer");
	app.getLayerStack().addLayer(&selectionLayer);

	SelectionLayerCallBack selectionLayerCallBack;
	viewPortsHolder->observe(&selectionLayer, &selectionLayerCallBack);

	selectionLayer.addObserver(&selectionLayerCallBack);
	selectionLayerCallBack.observe(&selectionLayer, &selectionLayerCallBack);

	//toolBarLayerCallBack.observe(&toolBarLayer, &toolBarLayerCallBack);



	//Transform Layer
	//TransformLayer transformLayer(viewPortHolder, "TransformLayer");

	//TransformAddPlaneCallBack transformAddPlaneCallBack(&transformLayer, &addPlaneCallBack);

	//selectMeshCallBack.addObserver(&transformLayer);
	//TransformSelectMeshCallBack transformSelectMeshCallBack(&transformLayer, &selectMeshCallBack);
	//transformLayer.observe(&selectMeshCallBack, &transformSelectMeshCallBack);
	//addPlaneCallBack.addObserver(&transformLayer);
	//transformLayer.observe(&addPlaneCallBack, &transformAddPlaneCallBack);

	//SCULPT TOOL
	BasicSculptToolCommand basicSculptToolCommand;


	//AdditionLayer
	AdditionLayer additionLayer("AdditionLayer");
	app.getLayerStack().addLayer(&additionLayer);

	//RemovalLayer
	RemovalLayer removalLayer("RemovalLayer");
	app.getLayerStack().addLayer(&removalLayer);

	//SculptToolsLayer
	SculptToolsLayer sculptToolsLayer("SculptToolsLayer");
	app.getLayerStack().addLayer(&sculptToolsLayer);

	clearAABBData();

	for (auto& entry : scene.m_res.coordsOctreeMap) {
		const glm::vec3& key = entry.first;
		Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>& octree = entry.second;
		for (auto& octreeNode : octree) {
			octreeNode.getBounds();
			collectAABBData(octreeNode.getBounds());
		}
	}

	//toolBarLayer.addObserver(&viewPortLayer);
	glm::mat4 model = glm::mat4(1.0f);


	//mesh shader
	meshShader.bind();
	meshShader.setMat4("u_model", model);
	meshShader.unbind();

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
		projection = glm::perspective(glm::radians(camera->getState().zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), nearPlane, farPlane);
		linesShader.setMat4("u_projection", projection);

		glm::mat4 view = camera->getState().lookAtMatrix;
		linesShader.setMat4("u_view", view);
		
		//linesShader.setMat4("u_model", model);

		linesShader.unbind();


		//mesh shader
		meshShader.bind();
		meshShader.setMat4("u_projection", projection);
		meshShader.setMat4("u_view", view);
		//meshShader.setMat4("u_model", model);
		meshShader.unbind();

		camera->m_matrices.perspectiveMatrix = projection;
		camera->m_matrices.viewMatrix = view;

		ray = Ray::fromMousePos(*camera, app.getWindow());

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
			clearAABBData();


			
			for (auto& entry : scene.m_res.coordsOctreeMap) {
				const glm::vec3& key = entry.first;
				Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>& octree = entry.second;
				for (auto& octreeNode : octree) {
					octreeNode.getBounds();
					collectAABBData(octreeNode.getBounds());
				}
			}

		}

		int screenWidth = ViewPortsHolderContext::s_window->getScreenWidth();
		int screenHeight = ViewPortsHolderContext::s_window->getScreenHeight();
		glViewport(0, 0, screenWidth, screenHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		for (auto& [mesh, materialVertexMap] : Renderer::s_stageData.meshMatsMap)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//chcem zapisovat do depth bufferu
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LESS);

			//nechcem zapisovat do stencil bufferu
			glStencilMask(0x00);
			glDisable(GL_STENCIL_TEST);

			for (auto& [material, vertexVector] : materialVertexMap)
			{
				// Pass the vertexVector to some method
				material->getShader()->bind();
				Renderer::drawMesh(vertexVector);
				material->getShader()->unbind();
			}
		}

		//RENDER LINES
		for (auto& [mesh, linesVector] : Renderer::s_stageData.meshLinesMap)
		{
			linesShader.bind();
			Renderer::drawLines(linesVector);
			linesShader.unbind();
		}

		ViewPortsHolderContext::s_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_edgeShader->bind();
		
		for (const auto& [region, vertices] : Renderer::s_stageData.aabbVertsMap) {
			
			Renderer::drawBox(vertices);
		}
		ViewPortsHolderContext::s_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_edgeShader->unbind();

		app.run();
		
		app.getWindow().update();

		Input::resetFrameInput();
	}

	app.close();
	return 0;

}