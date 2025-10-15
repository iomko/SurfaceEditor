#include "Callbacks/CreatePrintStructureCallBack.h"
#include "Commands/CreatePrintCommand.h"
#include "UI/ModifiersLayer.h"
#include "UI/PrintableMeshSettingsPopUpLayer.h"
#define NOMINMAX  // Prevents Windows.h from defining min/max macros

#include <limits>
//#include <Windows.h>
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

#include "../Renderer/Shader.h"
#include "../Renderer/Buffers.h"
#include "../Structures/Octree.h"
#include <tiffio.h>
//import LayerSystem.Layer.ImGuiLayer;
//import Patterns.Observer;

#include "../Structures/HalfEdge.h"

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
#include "../Callbacks/MoveVertexCallBack.h"
#include "../Commands/MoveVertexCommand.h"

#include "../Callbacks/DeselectMeshCallBack.h"
#include "../Commands/DeselectMeshCommand.h"

#include "../Commands/DeselectFaceCommand.h"
#include "../Commands/SelectFaceCommand.h"

#include "../Callbacks/DeleteSelectedFacesCallBack.h"
#include "../Commands/DeleteSelectedFacesCommand.h"

#include "../Callbacks/DeleteFaceCallBack.h"
#include "../Commands/DeleteFaceCommand.h"

#include "../Callbacks/DeleteSelectedMeshesCallBack.h"
#include "../Commands/DeleteSelectedMeshesCommand.h"

#include "../Commands/BasicSculptToolCommand.h"

#include "../Commands/BrushToolCommand.h"
#include "../Tools/MeshDeselectionTool.h"
#include "../Tools/BrushTool.h"
#include "../Tools/FaceSelectionTool.h"
#include "../Tools/FaceDeselectionTool.h"

#include "../Callables/PlaneVertexGenCallable.h"
#include "../Callables/MeshVaoInitCallable.h"
#include "../Callables/SceneMeshAdderCallable.h"

//ImportExportLayer
#include "../Callbacks/ImportMeshesCallBack.h"
#include "../Commands/ImportMeshesCommand.h"
#include "../UI/ImportExportLayer.h"

#include "../Callbacks/ExportMeshesCallBack.h"
#include "../Commands/ExportMeshesCommand.h"

#include "../Callables/FetchedSurfaceVertexGenCallable.h"
#include "../Callbacks/FetchSurfaceCallBack.h"

#include "../Callbacks/SolidifyMeshesCallBack.h"
#include "../Commands/SolidifyMeshesCommand.h"
#include "../Commands/DeleteMeshCommand.h"
#include "../Callbacks/DeleteMeshCallBack.h"

#include "../Structures/ExtendedHalfEdge.h"

#include "../Utils/GeometryUtils.h"

#include "UI/OutlinerLayer.h"
#include "Callables/MeshOutlinerAdderCallable.h"
#include "Structures/PrintableMesh.h"

#include "../Renderer/MaterialRegistry.h"

//INTERACTION_HANDLER
#include "../Tools/InteractionHandler.h"
#include "../Tools/Tool.h"

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;


std::string getShaderPath(const std::string& file){
    return std::string(SHADER_DIR) + "/" + file;
}


int main()
{
    
    WindowLayerBus windowLayerBus;

	CommandRegistry* commandRegistry = new CommandRegistry();

	//--INITIALIZATIONS_OF_FUNCTION_COMPOSERS--

	//--ADD_PLANE_COMPOSER--

	FunctionComposer addPlaneComposer;
	FunctionNode* addPlaneRoot = addPlaneComposer.initRoot<PlaneVertexGenCallable>();
	addPlaneComposer.addFunc<MeshVaoInitCallable>(addPlaneRoot);
	addPlaneComposer.addFunc<SceneMeshAdderCallable>(addPlaneRoot);
    addPlaneComposer.addFunc<MeshOutlinerAdderCallable>(addPlaneRoot);
	AddPlaneCallback addPlaneCallBack(addPlaneComposer);

    SolidifyMeshesCallBack solidifyMeshesCallBack;
    CreatePrintStructureCallBack createPrintStructureCallBack;



	//---FETCH_SURFACE_COMPOSER---
	FunctionComposer fetchSurfaceComposer;
	FunctionNode* fetchSurfaceRoot = fetchSurfaceComposer.initRoot<FetchedSurfaceVertexGenCallable>();
	fetchSurfaceComposer.addFunc<MeshVaoInitCallable>(fetchSurfaceRoot);
	fetchSurfaceComposer.addFunc<SceneMeshAdderCallable>(fetchSurfaceRoot);
    fetchSurfaceComposer.addFunc<MeshOutlinerAdderCallable>(fetchSurfaceRoot);
	FetchSurfaceCallBack fetchSurfaceCallBack(fetchSurfaceComposer);


	Application& app = Application::getInstance(SCR_WIDTH, SCR_HEIGHT, "SurfaceEditor");
	Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Shader linesShader("src/Renderer/Shaders/linesShader.vert", "src/Renderer/Shaders/linesShader.frag");
    Shader linesShader(getShaderPath("linesShader.vert"), getShaderPath("linesShader.frag"));
	//Shader meshShader = Shader("src/Renderer/Shaders/meshShader.vert", "src/Renderer/Shaders/meshShader.frag");
    Shader meshShader(getShaderPath("meshShader.vert"), getShaderPath("meshShader.frag"));

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
	ViewPortsHolderContext::s_viewPortsController = viewPortsHolder;
	ViewPortsHolderContext::s_selectionController = selectionController;
	ViewPortsHolderContext::s_camera = camera;
	ViewPortsHolderContext::s_window = app.window;

	//ViewPortLayer
	app.getLayerStack().addLayer(viewPortLayer);

	//ImporExportLayer
	ImportExportLayer* importExportLayer = new ImportExportLayer("importExportLayer", *commandRegistry);
	app.getLayerStack().addLayer(importExportLayer);

	//importMeshesCommand

	commandRegistry->registerCommand<ImportMeshesCommand>();
	ImportMeshesCommand* importMeshesCommand = commandRegistry->getCommand<ImportMeshesCommand>();

	ImportMeshesCallback importMeshesCallback;
	//viewPortHolder->observe(importMeshesCommand, &importMeshesCallBack);

	importMeshesCommand->addObserver(&importMeshesCallback);

	//importMeshesCommand->addObserver(&importMeshesCallBack);
	importMeshesCallback.observe(importMeshesCommand, &importMeshesCallback);
	//importMeshesCallBack.observe(importMeshesCommand, &importMeshesCallBack);

	commandRegistry->registerCommand<ExportMeshesCommand>();
	ExportMeshesCommand* exportMeshesCommand = commandRegistry->getCommand<ExportMeshesCommand>();

	ExportMeshesCallback exportMeshesCallback;
	//viewPortHolder->observe(exportMeshesCommand, &exportMeshesCallBack);

	exportMeshesCommand->addObserver(&exportMeshesCallback);
	exportMeshesCallback.observe(exportMeshesCommand, &exportMeshesCallback);

	commandRegistry->registerCommand<AddPlaneCommand>();
	AddPlaneCommand* addPlaneCommand = commandRegistry->getCommand<AddPlaneCommand>();

	addPlaneCommand->addObserver(&addPlaneCallBack);
	addPlaneCallBack.observe(addPlaneCommand, &addPlaneCallBack);

    commandRegistry->registerCommand<SolidifyMeshesCommand>();
    SolidifyMeshesCommand* solidifyMeshesCommand = commandRegistry->getCommand<SolidifyMeshesCommand>();

    solidifyMeshesCommand->addObserver(&solidifyMeshesCallBack);
    solidifyMeshesCallBack.observe(solidifyMeshesCommand, &solidifyMeshesCallBack);

    commandRegistry->registerCommand<CreatePrintCommand>();
    CreatePrintCommand* createPrintCommand = commandRegistry->getCommand<CreatePrintCommand>();
    createPrintCommand->addObserver(&createPrintStructureCallBack);
    createPrintStructureCallBack.observe(createPrintCommand, &createPrintStructureCallBack);

	commandRegistry->registerCommand<FetchSurfaceCommand>();
	FetchSurfaceCommand* fetchSurfaceCommand = commandRegistry->getCommand<FetchSurfaceCommand>();

	fetchSurfaceCommand->addObserver(&fetchSurfaceCallBack);
	fetchSurfaceCallBack.observe(fetchSurfaceCommand, &fetchSurfaceCallBack);

	SelectMeshCallBack selectMeshCallBack;
	commandRegistry->registerCommand<SelectMeshCommand>();
	SelectMeshCommand* selectMeshCommand = commandRegistry->getCommand<SelectMeshCommand>();

	ToolRegistry::registerTool<MeshSelectionTool>(selectMeshCommand);

	ToolRegistry::getTool<MeshSelectionTool>();

	selectMeshCommand->addObserver(&selectMeshCallBack);
	selectMeshCallBack.observe(selectMeshCommand, &selectMeshCallBack);

	BrushToolCallBack brushToolCallBack(commandRegistry);
	commandRegistry->registerCommand<BrushToolCommand>();
	BrushToolCommand* brushToolCommand = commandRegistry->getCommand<BrushToolCommand>();

	brushToolCommand->addObserver(&brushToolCallBack);
	brushToolCallBack.observe(brushToolCommand, &brushToolCallBack);

	//ToolRegistry::registerTool<BrushTool>(brushToolCommand, new BrushInteractionHandler());
	ToolRegistry::registerTool<BrushTool>(brushToolCommand);

	DeselectMeshCallBack deselectMeshCallBack;
	commandRegistry->registerCommand<DeselectMeshCommand>();
	DeselectMeshCommand* deselectMeshCommand = commandRegistry->getCommand<DeselectMeshCommand>();

    ToolRegistry::registerTool<MeshDeselectionTool>(deselectMeshCommand);

	deselectMeshCommand->addObserver(&deselectMeshCallBack);
	deselectMeshCallBack.observe(deselectMeshCommand, &deselectMeshCallBack);

	SelectFaceCallBack selectFaceCallBack;
	commandRegistry->registerCommand<SelectFaceCommand>();
	SelectFaceCommand* selectFaceCommand = commandRegistry->getCommand<SelectFaceCommand>();
	selectFaceCommand->addObserver(&selectFaceCallBack);
	selectFaceCallBack.observe(selectFaceCommand, &selectFaceCallBack);


    MoveVertexCallBack moveVertexCallBack;
	commandRegistry->registerCommand<MoveVertexCommand>();
	MoveVertexCommand* moveVertexCommand = commandRegistry->getCommand<MoveVertexCommand>();
	moveVertexCommand->addObserver(&moveVertexCallBack);
	moveVertexCallBack.observe(moveVertexCommand, &moveVertexCallBack);


	ToolRegistry::registerTool<FaceSelectionTool>(selectFaceCommand);

	DeselectFaceCallBack deselectFaceCallBack;
	commandRegistry->registerCommand<DeselectFaceCommand>();
	DeselectFaceCommand* deselectFaceCommand = commandRegistry->getCommand<DeselectFaceCommand>();
	deselectFaceCommand->addObserver(&deselectFaceCallBack);
	deselectFaceCallBack.observe(deselectFaceCommand, &deselectFaceCallBack);

	ToolRegistry::registerTool<FaceDeselectionTool>(deselectFaceCommand);


	DeleteFaceCallBack deleteFaceCallBack;
	commandRegistry->registerCommand<DeleteFaceCommand>();
	DeleteFaceCommand* deleteFaceCommand = commandRegistry->getCommand<DeleteFaceCommand>();
	deleteFaceCommand->addObserver(&deleteFaceCallBack);
	deleteFaceCallBack.observe(deleteFaceCommand, &deleteFaceCallBack);


	DeleteMeshCallBack deleteMeshCallBack(commandRegistry);
	commandRegistry->registerCommand<DeleteMeshCommand>();
	DeleteMeshCommand* deleteMeshCommand = commandRegistry->getCommand<DeleteMeshCommand>();
	deleteMeshCommand->addObserver(&deleteMeshCallBack);
	deleteMeshCallBack.observe(deleteMeshCommand, &deleteMeshCallBack);


	DeleteSelectedFacesCallBack deleteSelectedFacesCallBack(commandRegistry);
	commandRegistry->registerCommand<DeleteSelectedFacesCommand>();
	DeleteSelectedFacesCommand* deleteSelectedFacesCommand = commandRegistry->getCommand<DeleteSelectedFacesCommand>();
	deleteSelectedFacesCommand->addObserver(&deleteSelectedFacesCallBack);
	deleteSelectedFacesCallBack.observe(deleteSelectedFacesCommand, &deleteSelectedFacesCallBack);

	DeleteSelectedMeshesCallBack deleteSelectedMeshesCallBack(commandRegistry);
	commandRegistry->registerCommand<DeleteSelectedMeshesCommand>();
	DeleteSelectedMeshesCommand* deleteSelectedMeshesCommand = commandRegistry->getCommand<DeleteSelectedMeshesCommand>();
	deleteSelectedMeshesCommand->addObserver(&deleteSelectedMeshesCallBack);
	deleteSelectedMeshesCallBack.observe(deleteSelectedMeshesCommand, &deleteSelectedMeshesCallBack);

	SelectionLayer selectionLayer("SelectionLayer");
	app.getLayerStack().addLayer(&selectionLayer);

	SelectionLayerCallBack selectionLayerCallBack;
	viewPortsHolder->observe(&selectionLayer, &selectionLayerCallBack);

	selectionLayer.addObserver(&selectionLayerCallBack);
	selectionLayerCallBack.observe(&selectionLayer, &selectionLayerCallBack);

	//SCULPT TOOL
	BasicSculptToolCommand basicSculptToolCommand;


	//AdditionLayer
	AdditionLayer additionLayer("AdditionLayer", *commandRegistry);
	app.getLayerStack().addLayer(&additionLayer);

    //OutlinerLayer
    OutlinerLayer outlinerLayer("OutlinerLayer", windowLayerBus);
    app.getLayerStack().addLayer(&outlinerLayer);

    //popUpLayer
    PrintableMeshSettingsPopUpLayer popUpLayer("PopUpLayer", *commandRegistry, windowLayerBus);
    app.getLayerStack().addLayer(&popUpLayer);


	//ModifiersLayer
	ModifiersLayer modifiersLayer("ModifiersLayer", *commandRegistry, windowLayerBus);
	app.getLayerStack().addLayer(&modifiersLayer);

	//RemovalLayer
	RemovalLayer removalLayer("RemovalLayer", *commandRegistry);
	app.getLayerStack().addLayer(&removalLayer);

	//SculptToolsLayer
	SculptToolsLayer sculptToolsLayer("SculptToolsLayer");
	app.getLayerStack().addLayer(&sculptToolsLayer);

	glm::mat4 model = glm::mat4(1.0f);


	//mesh shader
	meshShader.bind();
	meshShader.setMat4("u_model", model);
	meshShader.unbind();

	//lines shader
	linesShader.bind();
	linesShader.setMat4("u_model", model);
	linesShader.unbind();

    
    MaterialRegistry::registerMaterial("defaultMeshMaterial", &meshShader);
    MaterialRegistry::registerMaterial("defaultLineMaterial", &linesShader);

	while (!glfwWindowShouldClose(app.getWindow().getWindowHandle()))
	{

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

		int screenWidth = ViewPortsHolderContext::s_window->getScreenWidth();
		int screenHeight = ViewPortsHolderContext::s_window->getScreenHeight();
		glViewport(0, 0, screenWidth, screenHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //chcem zapisovat do depth bufferu
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        //draw meshes
        for (auto& [mesh, _] : scene.m_res.meshFaceOctreeCoordsMap) {

            for (auto it = mesh->bufferLayout.triangleBuffersBegin(); it != mesh->bufferLayout.triangleBuffersEnd(); ++it) {
                Shader* shader = it->first->m_shader;
                BufferStorageData<BufferStorageDataType::TriangleVertex>& triangleBufferData = it->second.data;
                Renderer::drawTriangles(triangleBufferData, shader);
            }
            
            for (auto it = mesh->bufferLayout.lineBuffersBegin(); it != mesh->bufferLayout.lineBuffersEnd(); ++it) {
                Shader* shader = it->first->m_shader;
                BufferStorageData<BufferStorageDataType::LineVertex>& lineBufferData = it->second.data;
                Renderer::drawLines(lineBufferData, shader);
            }
        }

        //draw printableMeshes
        for(auto& [_, printableMesh] : scene.m_res.printableMeshMap) {

            for (auto it = printableMesh->bufferLayout.lineBuffersBegin(); it != printableMesh->bufferLayout.lineBuffersEnd(); ++it) {
                Shader* shader = it->first->m_shader;
                BufferStorageData<BufferStorageDataType::LineVertex>& lineBufferData = it->second.data;
                Renderer::drawLines(lineBufferData, shader);
            }
        }

		app.run();
		app.getWindow().update();
		Input::updateButtonClicks();
	}

	app.close();
	return 0;
}
