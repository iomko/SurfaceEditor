#define NOMINMAX // Prevents Windows.h from defining min/max macros
#include <limits>
// #include <Windows.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tiffio.h>
#include "Input.h"
#include "../Primitives/AABB.h"
#include "../Core/Application.h"
#include "../Core/Input.h"
#include "../Core/Window.h"
// #include "../Mesh.h"
#include "../Scene/Mesh.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Buffers.h"
#include "../Structures/Octree.h"
#include "../Structures/ExtendedHalfEdge.h"
#include "../Structures/HalfEdge.h"
#include "../Structures/PrintableMesh.h"
// import LayerSystem.Layer.ImGuiLayer;
// import Patterns.Observer;
// #include "../Patterns/Observer.h"
#include "../ViewPortsController.h"
#include "../Callbacks/AddPlaneCallback.h"
#include "../Commands/AddPlaneCommand.h"
#include "../Tools/ToolRegistry.h"
//#include "../Commands/CommandRegistry.h"
#include "../Commands/AddCubeCommand.h"
#include "../Callbacks/AddCubeCallback.h"

#include "../Callbacks/DeselectFaceCallBack.h"
#include "../Callbacks/SelectMeshCallBack.h"
#include "../Commands/SelectMeshCommand.h"
#include "../UI/AdditionLayer.h"
#include "../UI/RemovalLayer.h"

#include "../Callbacks/SelectFaceCallBack.h"
#include "../Callbacks/MoveVertexCallBack.h"
#include "../Commands/MoveVertexCommand.h"

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
#include "../Callables/CubeVertexGenCallable.h"

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
#include "../Callbacks/MoveSelectedFacesCallBack.h"
#include "../Commands/MoveSelectedFacesCommand.h"

#include "../Structures/ExtendedHalfEdge.h"

#include "../Utils/GeometryUtils.h"
#include "../Renderer/MaterialRegistry.h"
// INTERACTION_HANDLER
#include <string>
#include "../Callbacks/CallbackRegister.h"
#include "../Callbacks/CallbackIDs.h"
#include "../Tools/ToolRegistry.h"
#include "../Tools/ToolIDs.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"
#include "../UI/LayerRegistry.h"
#include "../UI/LayerIDs.h"

#include "../UI/WindowLayerBus.h"

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

std::string getShaderPath(const std::string &file)
{
#ifdef SHADER_DIR
	return std::string(SHADER_DIR) + "/" + file;
#else
	return "../Renderer/Shaders/" + file;
#endif
}

static void setup(const int command_id, const int callback_id, const int tool_id = -1)
{
	auto *callback = CallbackRegistry::instance().getCallback(callback_id);
	if (callback == nullptr)
	{
		printf("not callback with id %d\n", callback_id);
		return;
	}
	auto *command = CommandRegistry::instance().getCommand(command_id); // zjednotit + osobitny .h ako ciselnik a robit cez id
	auto *observableCommand = dynamic_cast<Observable *>(command);
	auto *observerCallback = dynamic_cast<Observer *>(callback);
	if (observableCommand && observerCallback)
	{
		observableCommand->addObserver(observerCallback);
		observerCallback->observe(observableCommand, callback);
		if (tool_id != -1)
		{
			ToolRegistry::instance().initializeTool(tool_id, command);
		}
	}
}

static void setupLayer(const int layer_id, Application &app, const std::string name, WindowLayerBus *bus = nullptr)
{
	Layer *layer;
	if (bus == nullptr)
	{
		layer = LayerRegistry::instance().getLayer(layer_id, name);
	}
	else
	{
		layer = LayerRegistry::instance().getLayer(layer_id, name, std::ref(*bus));
	}
	if (layer)
		app.getLayerStack().addLayer(layer);
}

int main()
{

	WindowLayerBus windowLayerBus;

	Application &app = Application::getInstance(SCR_WIDTH, SCR_HEIGHT, "SurfaceEditor");

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Shader linesShader("src/Renderer/Shaders/linesShader.vert", "src/Renderer/Shaders/linesShader.frag");
	Shader linesShader(getShaderPath("linesShader.vert"), getShaderPath("linesShader.frag"));
	// Shader meshShader = Shader("src/Renderer/Shaders/meshShader.vert", "src/Renderer/Shaders/meshShader.frag");
	Shader meshShader(getShaderPath("meshShader.vert"), getShaderPath("meshShader.frag"));

	ViewPortsHolderContext::s_selectionController->createSelectionRectangle();

	Camera* camera = ViewPortsHolderContext::s_camera.get();
	Scene scene(25.0f, 25.0f, 25.0f);

	// viewPortLayer
	ViewPortLayer* viewPortLayer = new ViewPortLayer("viewPortLayer");
	viewPortLayer->m_camera = camera;

	viewPortLayer->m_shaderSettings.m_faceShader = &meshShader;
	viewPortLayer->m_shaderSettings.m_meshShader = &meshShader;
	viewPortLayer->m_shaderSettings.m_edgeShader = &linesShader;

	ViewPortsController* viewPortsHolder = ViewPortsHolderContext::s_viewPortsController.get();
	viewPortsHolder->addLayer(viewPortLayer);
	viewPortsHolder->m_activeViewPortLayer = viewPortLayer;

	viewPortsHolder->m_scene = &scene;
	ViewPortsHolderContext::s_window = app.window;

	// ViewPortLayer
	app.getLayerStack().addLayer(viewPortLayer);

	// ImporExportLayer
	setupLayer(IMPORT_EXPORT_LAYER, app, "ImportExportLayer");

	// GizmoLayer
	setupLayer(GIZMO_LAYER, app, "GizmoLayer");

	setup(MOVE_MESH_COMMAND, MOVE_MESH_CALLBACK);
	setup(MOVE_SELECTED_MESHES_COMMAND, MOVE_SELECTED_MESHES_CALLBACK);
	setup(HANDLE_GIZMO_COMMAND, HANDLE_GIZMO_CALLBACK);

	setup(IMPORT_MESHES_COMMAND, IMPORT_MESHES_CALLBACK);
	setup(EXPORT_MESHES_COMMAND, EXPORT_MESHES_CALLBACK);

	setup(ADD_PLANE_COMMAND, ADD_PLANE_CALLBACK);

	setup(ADD_CUBE_COMMAND, ADD_CUBE_CALLBACK);

	setup(SOLIDIFY_MESHES_COMMAND, SOLIDIFY_MESHES_CALLBACK);

	setup(CREATE_PRINT_COMMAND, CREATE_PRINT_STRUCTURE_CALLBACK);

	setup(FETCH_SURFACE_COMMAND, FETCH_SURFACE_CALLBACK);

	setup(SELECT_MESH_COMMAND, SELECT_MESH_CALLBACK, MESH_SELECTION_TOOL);

	setup(BRUSH_TOOL_COMMAND, BRUSH_TOOL_CALLBACK, BRUSH_TOOL);

	setup(DESELECT_MESH_COMMAND, DESELECT_MESH_CALLBACK, MESH_DESELECTION_TOOL);

	setup(SELECT_FACE_COMMAND, SELECT_FACE_CALLBACK, FACE_SELECTION_TOOL);
	setup(MOVE_VERTEX_COMMAND, MOVE_VERTEX_CALLBACK);

	setup(MOVE_SELECTED_FACES_COMMAND, MOVE_SELECTED_FACES_CALLBACK);

	setup(DESELECT_FACE_COMMAND, DESELECT_FACE_CALLBACK, FACE_DESELECTION_TOOL);

	setup(DELETE_FACE_COMMAND, DELETE_FACE_CALLBACK);

	setup(DELETE_MESH_COMMAND, DELETE_MESH_CALLBACK);

	setup(DELETE_SELECTED_FACES_COMMAND, DELETE_SELECTED_FACES_CALLBACK);

	setup(DELETE_SELECTED_MESHES_COMMAND, DELETE_SELECTED_MESHES_CALLBACK);

	setupLayer(ADDITION_LAYER, app, "AdditionLayer");

	Layer* outlinerLayer = LayerRegistry::instance().getLayer(OUTLINER_LAYER, std::string("OutlinerLayer") , std::ref(windowLayerBus));
	
	if (outlinerLayer)
	{
		app.getLayerStack().addLayer(outlinerLayer);		
		Observer* outlinerObserver = dynamic_cast<Observer*>(outlinerLayer);
		if(outlinerObserver)
		{
			int id = TemplateOutlinerNodeAdderCallbackIDManger::instance().GetIndex<Mesh>(false);
			auto* addNewOutlinerNodeCallBackMesh = CallbackRegistry::instance().getCallback(id);
			printf("Getting template on id %d\n", id);
			if(addNewOutlinerNodeCallBackMesh)
			{
				Observable* addNewOutlinerNodeMeshObservalbe = dynamic_cast<Observable*>(addNewOutlinerNodeCallBackMesh);
				addNewOutlinerNodeMeshObservalbe->addObserver(outlinerObserver);
				outlinerObserver->observe(addNewOutlinerNodeMeshObservalbe, addNewOutlinerNodeCallBackMesh);
			}
			id = TemplateOutlinerNodeAdderCallbackIDManger::instance().GetIndex<PrintableMesh>(true);
			auto* addChildOutlinerNodeCallBackPrintableMesh = CallbackRegistry::instance().getCallback(id);
			printf("Getting template on id %d\n", id);
			
			if(addChildOutlinerNodeCallBackPrintableMesh)
			{
				Observable* addChildOutlinerNodePrintableMeshObservable = dynamic_cast<Observable*>(addChildOutlinerNodeCallBackPrintableMesh);
				addChildOutlinerNodePrintableMeshObservable->addObserver(outlinerObserver);
				outlinerObserver->observe(addChildOutlinerNodePrintableMeshObservable, addChildOutlinerNodeCallBackPrintableMesh);
			}
		}
	}
	//setupLayer(OUTLINER_LAYER, app, "OutlinerLayer", &windowLayerBus);

	setupLayer(PRINTABLE_MESH_SETTINGS_POP_UP_LAYER, app, "PopUpLayer", &windowLayerBus);

	setupLayer(MODIFIERS_LAYER, app, "ModifiersLayer", &windowLayerBus);

	setupLayer(REMOVAL_LAYER, app, "RemovalLayer");

	setupLayer(SCULPT_TOOLS_LAYER, app, "SculptToolsLayer");

	setupLayer(DEBUG_LAYER, app, "DebugLayer");

	setupLayer(OBJECT_MANIPULATION_LAYER, app, "ObjectManipulationLayer");

	setupLayer(OBJECTS, app, "ObjectsLayer");

	glm::mat4 model = glm::mat4(1.0f);
	// mesh shader
	meshShader.bind();
	meshShader.setMat4("u_model", model);
	meshShader.unbind();

	// lines shader
	linesShader.bind();
	linesShader.setMat4("u_model", model);
	linesShader.unbind();

	MaterialRegistry::registerMaterial("defaultMeshMaterial", &meshShader);
	MaterialRegistry::registerMaterial("defaultLineMaterial", &linesShader);

	auto drawMesh = [&](Mesh* mesh, const SelectionHolder& selectionHolder)
	{
		auto& faces = selectionHolder.faces.find(mesh)->second;

		for (auto it = mesh->bufferLayout.triangleBuffersBegin(); it != mesh->bufferLayout.triangleBuffersEnd(); ++it)
		{
			Shader* shader = it->first->m_shader;

			glm::mat4 model;
			if (faces.empty() && mesh->m_realTimeTransform != nullptr && mesh->m_selected)
			{
				model = *mesh->m_realTimeTransform;
			}
			else
			{
				model = glm::mat4(1.0f);
			}

			shader->bind();
			shader->setMat4("u_model", model);

			Renderer::drawTriangles(it->second.data, shader);

			shader->unbind();
		}
	};

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

		// linesShader.setMat4("u_model", model);

		linesShader.unbind();

		// mesh shader
		meshShader.bind();
		meshShader.setMat4("u_projection", projection);
		meshShader.setMat4("u_view", view);
		// meshShader.setMat4("u_model", model);
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
        glDepthFunc(GL_LESS);

		const SelectionHolder& selectionHolder = ViewPortsHolderContext::s_selectionController->getHolder();

		//Render transparent meshes
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		for (auto& [mesh, _] : scene.m_res.meshFaceOctreeCoordsMap)
		{
			if (mesh->m_selected)
			{
				continue;
			}

			drawMesh(mesh, selectionHolder);
		}
		//Render solid meshes
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		for (auto& [mesh, _] : scene.m_res.meshFaceOctreeCoordsMap)
		{
			if (!mesh->m_selected)
			{
				continue;
			}

			drawMesh(mesh, selectionHolder);
		}
		// restore state
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		for (auto& [mesh, _] : scene.m_res.meshFaceOctreeCoordsMap)
		{
			auto& faces = selectionHolder.faces.find(mesh)->second;

			for (auto it = mesh->bufferLayout.lineBuffersBegin(); it != mesh->bufferLayout.lineBuffersEnd(); ++it)
			{
				Shader* shader = it->first->m_shader;

				glm::mat4 model;
				if (faces.empty() && mesh->m_realTimeTransform != nullptr && mesh->m_selected)
				{
					model = *mesh->m_realTimeTransform;
				}
				else
				{
					model = glm::mat4(1.0f);
				}

				shader->bind();
				shader->setMat4("u_model", model);

				Renderer::drawLines(it->second.data, shader);

				shader->unbind();
			}
		}

		for (auto& [_, printableMesh] : scene.m_res.printableMeshMap)
		{
			for (auto it = printableMesh->bufferLayout.lineBuffersBegin(); it != printableMesh->bufferLayout.lineBuffersEnd(); ++it)
			{
				Shader* shader = it->first->m_shader;
				Renderer::drawLines(it->second.data, shader);
			}
		}

		ViewPortsHolderContext::s_selectionController->drawSelectionRectangle();

		app.run();
		app.getWindow().update();
		Input::updateButtonClicks();
	}

	app.close();
	return 0;
}
