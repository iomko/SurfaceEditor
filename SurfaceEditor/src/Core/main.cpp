#include <iostream>
#include <vector>
#include "../Core/Window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Core/Application.h"
#include "../Events/Mouse.h"
#include "../TriangleData.h"
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
import Geometry.Primitive.Cube;



float deltaTime = 0.0f;
float lastFrame = 0.0f;


Ray getCameraRay(const glm::vec3& cameraPosition, const glm::vec3& cameraFrontVector)
{
	return Ray(cameraPosition, cameraFrontVector);
}

#include "../DataStructures/HalfEdge.h"

//#include "../Patterns/Observer.h"
#include "../ViewPortHolder.h"
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

#include "../ShortcutRegistry.h"

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
	ViewPortLayer* viewPortLayer = new ViewPortLayer("viewPortLayer", &camera, &scene);
	viewPortLayer->m_meshShader = &meshShader;
	viewPortLayer->m_normalsShader = &normalsShader;
	viewPortLayer->m_shaderSingleColor = &shaderSingleColor;
	viewPortLayer->m_pointsShader = &pointsShader;
	viewPortLayer->m_linesShader = &linesShader;
	viewPortLayer->m_shaderBlit = &shaderBlit;
	viewPortLayer->m_shaderDilation = &shaderDilation;
	app.getLayerStack().addLayer(viewPortLayer);

	ViewPortHolder* viewPortHolder = new ViewPortHolder(viewPortLayer);
	PlaneProperties planeProperties;
	planeProperties.m_size = 50.0f;
	planeProperties.m_subdivisionLevel = 4;


	//ImporExportLayer
	ImportExportLayer* importExportLayer = new ImportExportLayer(viewPortHolder, "importExportLayer");
	app.getLayerStack().addLayer(importExportLayer);

	//importMeshesCommand

	CommandRegistry::registerCommand<ImportMeshesCommand>(viewPortHolder);
	ImportMeshesCommand* importMeshesCommand = CommandRegistry::getCommand<ImportMeshesCommand>();

	ImportMeshesCallBack importMeshesCallBack(viewPortHolder, importMeshesCommand);
	viewPortHolder->observe(importMeshesCommand, &importMeshesCallBack);

	CommandRegistry::registerCommand<ExportMeshesCommand>(viewPortHolder);
	ExportMeshesCommand* exportMeshesCommand = CommandRegistry::getCommand<ExportMeshesCommand>();

	ExportMeshesCallBack exportMeshesCallBack(viewPortHolder, exportMeshesCommand);
	viewPortHolder->observe(exportMeshesCommand, &exportMeshesCallBack);

	CommandRegistry::registerCommand<AddPlaneCommand>(viewPortHolder, planeProperties);
	AddPlaneCommand* addPlaneCommand = CommandRegistry::getCommand<AddPlaneCommand>();
	
	AddPlaneCallback addPlaneCallBack(viewPortHolder, addPlaneCommand);
	viewPortHolder->observe(addPlaneCommand, &addPlaneCallBack);

	SelectMeshCallBack selectMeshCallBack(viewPortHolder);
	CommandRegistry::registerCommand<SelectMeshCommand>(viewPortHolder);
	SelectMeshCommand* selectMeshCommand = CommandRegistry::getCommand<SelectMeshCommand>();
	viewPortHolder->observe(selectMeshCommand, &selectMeshCallBack);

	DeselectMeshCallBack deselectMeshCallBack(viewPortHolder);
	CommandRegistry::registerCommand<DeselectMeshCommand>(viewPortHolder);
	DeselectMeshCommand* deselectMeshCommand = CommandRegistry::getCommand<DeselectMeshCommand>();
	viewPortHolder->observe(deselectMeshCommand, &deselectMeshCallBack);

	SelectFaceCallBack selectFaceCallBack(viewPortHolder);
	CommandRegistry::registerCommand<SelectFaceCommand>(viewPortHolder);
	SelectFaceCommand* selectFaceCommand = CommandRegistry::getCommand<SelectFaceCommand>();
	viewPortHolder->observe(selectFaceCommand, &selectFaceCallBack);

	SelectVertexCallBack selectVertexCallBack(viewPortHolder);
	CommandRegistry::registerCommand<SelectVertexCommand>(viewPortHolder);
	SelectVertexCommand* selectVertexCommand = CommandRegistry::getCommand<SelectVertexCommand>();
	viewPortHolder->observe(selectVertexCommand, &selectVertexCallBack);


	ToolBarLayer toolBarLayer(viewPortHolder, "ToolBarLayer");
	app.getLayerStack().addLayer(&toolBarLayer);

	ToolBarLayerCallBack toolBarLayerCallBack(viewPortHolder, &toolBarLayer);
	viewPortHolder->observe(&toolBarLayer, &toolBarLayerCallBack);

	//Transform Layer
	TransformLayer transformLayer(viewPortHolder, "TransformLayer");

	TransformAddPlaneCallBack transformAddPlaneCallBack(&transformLayer, &addPlaneCallBack);

	//potrebujem tomuto TransformLayeru pridat observable
	//ma observovat AddPlaneCallBack
	//ma observovat dalej aj SelectMeshCallBack

	selectMeshCallBack.addObserver(&transformLayer);
	TransformSelectMeshCallBack transformSelectMeshCallBack(&transformLayer, &selectMeshCallBack);
	transformLayer.observe(&selectMeshCallBack, &transformSelectMeshCallBack);

	addPlaneCallBack.addObserver(&transformLayer);
	transformLayer.observe(&addPlaneCallBack, &transformAddPlaneCallBack);


	CommandRegistry::registerCommand<TransformMeshCommand>(viewPortHolder);
	TransformMeshCommand* transformMeshCommand = CommandRegistry::getCommand<TransformMeshCommand>();
	TransformMeshCallBack transformMeshCallBack(viewPortHolder,transformMeshCommand);
	viewPortHolder->observe(transformMeshCommand, &transformMeshCallBack);



	//SCULPT TOOL
	BasicSculptToolCommand basicSculptToolCommand(viewPortHolder);


	

	//SHORTCUTREGISTRY
	
	ShortcutRegistry::registerShortcut({ GLFW_KEY_B, GLFW_KEY_A, GLFW_KEY_C }, selectMeshCommand);


	//AdditionLayer
	AdditionLayer additionLayer("AdditionLayer");
	app.getLayerStack().addLayer(&additionLayer);

	//TransformLayer
	app.getLayerStack().addLayer(&transformLayer);


	for (auto& entry : scene.m_meshFaceOctreesMap) {
		const glm::vec3& key = entry.first;
		Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>& octree = entry.second;
		for (auto& octreeNode : octree) {
			octreeNode.getBounds();
			Renderer::collectAABBdata(octreeNode.getBounds(), glm::vec3{ 0.0,1.0,0.0 });
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
			Renderer::deleteAABBBuffer();

			int numberOfOctrees = 0;
			for (auto& entry : scene.m_meshFaceOctreesMap) {
				++numberOfOctrees;
				const glm::vec3& key = entry.first;
				Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>& octree = entry.second;
				for (auto& octreeNode : octree) {
					octreeNode.getBounds();
					Renderer::collectAABBdata(octreeNode.getBounds(), glm::vec3{ 0.0,1.0,0.0 });
				}
			}

		}

		glViewport(0, 0, viewPortLayer->SCR_WIDTH, viewPortLayer->SCR_HEIGHT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		for (auto& [mesh, tuple] : viewPortHolder->m_meshesShaderData)
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
			Renderer::drawMesh(mesh->m_combinedVertexDataMatVector.m_vertexData);
			std::get<1>(tuple).FaceShader.unbind();

			//for normals
			std::get<1>(tuple).NormalShader.bind();
			Renderer::drawMesh(mesh->m_combinedVertexDataMatVector.m_vertexData);
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

		app.run();

		
		app.getWindow().update();
	}

	app.close();
	return 0;

}