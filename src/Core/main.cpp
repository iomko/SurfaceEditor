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



#include "../Structures/ExtendedHalfEdge.h"

#include "../Utils/GeometryUtils.h"
#include "../Renderer/MaterialRegistry.h"
// INTERACTION_HANDLER
#include <string>
#include "../Callbacks/CallbackRegister.h"
#include "../Callbacks/CallbackIDs.h"
#include "../Tools/ToolRegistry.h"
#include "../UI/LayerRegistry.h"

#include "../Callables/CallableRegistry.h"

#include "../UI/WindowLayerBus.h"

#include "PluginLoader.h"

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

static void setupLayer(std::string layer_id, Application &app, const std::string name, WindowLayerBus *bus = nullptr)
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

//add all features for now here
//#include "../Ml/FeatureStrategies/FaceAreaFeature.h"
//#include "../Ml/FeatureStrategies/FaceHeightsFeature.h"
//#include "../Ml/FeatureStrategies/FaceInternalAnglesFeature.h"
//#include "../Ml/FeatureStrategies/FaceSideLengthRatioFeature.h"
//#include "../Ml/FeatureStrategies/FaceSideLengthRatioFeature.h"

int main()
{

	WindowLayerBus& windowLayerBus = WindowLayerBus::instance();
	std::cout << &windowLayerBus << std::endl;
	Application &app = Application::getInstance(SCR_WIDTH, SCR_HEIGHT, "SurfaceEditor");
	Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Shader linesShader("src/Renderer/Shaders/linesShader.vert", "src/Renderer/Shaders/linesShader.frag");
	Shader linesShader(getShaderPath("linesShader.vert"), getShaderPath("linesShader.frag"));
	// Shader meshShader = Shader("src/Renderer/Shaders/meshShader.vert", "src/Renderer/Shaders/meshShader.frag");
	Shader meshShader(getShaderPath("meshShader.vert"), getShaderPath("meshShader.frag"));

	Scene scene(25.0f, 25.0f, 25.0f);

	// viewPortLayer
	ViewPortLayer *viewPortLayer = new ViewPortLayer("viewPortLayer");
	viewPortLayer->m_camera = camera;

	viewPortLayer->m_shaderSettings.m_faceShader = &meshShader;
	viewPortLayer->m_shaderSettings.m_meshShader = &meshShader;
	viewPortLayer->m_shaderSettings.m_edgeShader = &linesShader;

	ViewPortsController *viewPortsHolder = new ViewPortsController();
	viewPortsHolder->addLayer(viewPortLayer);
	viewPortsHolder->m_activeViewPortLayer = viewPortLayer;
	SelectionController *selectionController = new SelectionController();

	viewPortsHolder->m_scene = &scene;
	ViewPortsHolderContext::s_viewPortsController = viewPortsHolder;
	ViewPortsHolderContext::s_selectionController = selectionController;
	ViewPortsHolderContext::s_camera = camera;
	ViewPortsHolderContext::s_window = app.window;

	// ViewPortLayer
	app.getLayerStack().addLayer(viewPortLayer);

	std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Aktuálny pracovný adresár: " << cwd << std::endl;

	//spravit aby sa to natiahlo uz pocas buildenia nieco ako ine veci ze tie so su nakopirovane do hlavneho build adresara

	PluginLoader::LoadPlugins("plugins");

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
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

		const SelectionHolder& selectionHolder = selectionController->getHolder();

        //draw meshes
        for (auto& [mesh, _] : scene.m_res.meshFaceOctreeCoordsMap) {			
			for (auto it = mesh->bufferLayout.triangleBuffersBegin(); it != mesh->bufferLayout.triangleBuffersEnd(); ++it) {
				Shader* shader = it->first->m_shader;
				auto& faces = selectionHolder.faces.find(mesh)->second;

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
				shader->unbind();

				BufferStorageData<BufferStorageDataType::TriangleVertex>& triangleBufferData = it->second.data;				
				Renderer::drawTriangles(triangleBufferData, shader);
            }
            
            for (auto it = mesh->bufferLayout.lineBuffersBegin(); it != mesh->bufferLayout.lineBuffersEnd(); ++it) {
				Shader *shader = it->first->m_shader;
				auto& faces = selectionHolder.faces.find(mesh)->second;

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
				shader->unbind();

				BufferStorageData<BufferStorageDataType::LineVertex>& lineBufferData = it->second.data;
				Renderer::drawLines(lineBufferData, shader);
            }
        }
        //draw printableMeshes
        for(auto& [_, printableMesh] : scene.m_res.printableMeshMap) {

			for (auto it = printableMesh->bufferLayout.lineBuffersBegin(); it != printableMesh->bufferLayout.lineBuffersEnd(); ++it)
			{
				Shader *shader = it->first->m_shader;
				BufferStorageData<BufferStorageDataType::LineVertex> &lineBufferData = it->second.data;
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
