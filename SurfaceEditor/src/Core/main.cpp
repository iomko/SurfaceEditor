#define NOMINMAX  // Prevents Windows.h from defining min/max macros


#include <limits>
#include <Windows.h>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Input.h"
#include "../AABBBoundingRegion.h"
#include "../Core/Application.h"
#include "../Core/Input.h"
#include "../Core/Window.h"
#include "../Renderer/Renderer.h"
//#include "../Mesh.h"
#include "../FunctionComposer.h"
#include "../PolygonTraits.h"
#include "../Renderer/Framebuffer.h"
#include "../Callbacks/BrushToolCallBack.h"
#include "../Gui/SculptToolsLayer.h"


import Renderer.Shader;
import Renderer.Buffers;
#include "../DataStructures/Octree.h"
#include "tiff/tiffio.h"
//import LayerSystem.Layer.ImGuiLayer;
//import Patterns.Observer;



float deltaTime = 0.0f;
float lastFrame = 0.0f;


#include "../DataStructures/HalfEdge.h"

//#include "../Patterns/Observer.h"
#include "../ViewPortsHolder.h"
#include "../Callbacks/AddPlaneCallback.h"
#include "../Commands/AddPlaneCommand.h"
#include "../Tools/ToolRegistry.h"
//#include "../Commands/CommandRegistry.h"


#include "../Callbacks/DeselectFaceCallBack.h"
#include "../Callbacks/SelectMeshCallBack.h"
#include "../Callbacks/SelectionLayerCallBack.h"
#include "../Commands/SelectMeshCommand.h"
#include "../Gui/AdditionLayer.h"
#include "../Gui/RemovalLayer.h"
#include "../Gui/SelectionLayer.h"

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


//#include "../OBJExporter.h"
//#include "../OBJImporter.h"

//ImportExportLayer
#include "../Callbacks/ImportMeshesCallback.h"
#include "../Commands/ImportMeshesCommand.h"
#include "../Gui/ImportExportLayer.h"

#include "../Callbacks/ExportMeshesCallback.h"
#include "../Commands/ExportMeshesCommand.h"

#include "../Callables/GenFetchedSurfaceVertexDataCallable.h"
#include "../Callbacks/FetchSurfaceCallBack.h"

//DELAUNAY_TEST
#include "../DelaunayTest.h"
#include "../Triangulator.h"

//INTERACTION_HANDLER
#include "../Tools/InteractionHandler.h"
#include "../Tools/Tool.h"

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;



//=====TESTING THE FUNCTION_COMPOSER=====


//=====PLANE_MESH_DATA=====

//OUTPUT



class RootInputParams : public Params {
public:
	int rootInputValue;

	~RootInputParams()
	{
		//toto by sa nemalo zavolat
		std::cout << "DESTRUCTED Root_Input_Params" << std::endl;
	}
};
	

class RootOutputParams : public Params {
public:
	int rootOutputValue;

	~RootOutputParams()
	{
		std::cout << "DESTRUCTED Root_Output_Params" << std::endl;
	}
};

class Add5OutputParams : public Params {
public:
	int add5OutputParams;

	~Add5OutputParams()
	{
		std::cout << "DESTRUCTED Add_50_Output_Params" << std::endl;
	}
};

class Add100OutputParams : public Params {
public:
	int add100OutputParams;

	~Add100OutputParams()
	{
		std::cout << "DESTRUCTED Add_100_Output_Params" << std::endl;
	}
};


//------

// RootCreationSquare - Calculates square of the rootInputValue
class RootCreationSquare : public Callable<RootInputParams, RootOutputParams> {
public:

	void invoke(const RootInputParams& input, RootOutputParams& output) override
	{
		output.rootOutputValue = input.rootInputValue * input.rootInputValue;
		std::cout << "CREATING_2" << std::endl;
		std::cout << "CREATING_2_OUTPUT: " << output.rootOutputValue << std::endl;
	}
};

// Add5 - Adds 5 to the rootOutputValue
class Add5 : public Callable<RootOutputParams, Add5OutputParams> {
public:
	void invoke(const RootOutputParams& input, Add5OutputParams& output) override
	{
		output.add5OutputParams = input.rootOutputValue + 5;
		std::cout << "ADDING_5" << std::endl;
	}
};

// Divide10 - Divides rootOutputValue by 10 and returns the result
class Divide10 : public Callable<RootOutputParams, void> {
public:
	void invoke(const RootOutputParams& input) override
	{
		std::cout << "Divided_By_10: " << input.rootOutputValue / 10 << std::endl;
	}
};

// Multiply10 - Multiplies rootOutputValue by 10 and returns the result
class Multiply10 : public Callable<RootOutputParams, void> {
public:

	void invoke(const RootOutputParams& input) override
	{
		std::cout << "Multiplied_By_10: " << input.rootOutputValue * 10 << std::endl;
	}
};

// Subtract10 - Subtracts 10 from rootOutputValue and returns the result
class Subtract10 : public Callable<RootOutputParams, void> {
public:

	void invoke(const RootOutputParams& input) override
	{
		std::cout << "Subtracted_By_10: " << input.rootOutputValue - 10 << std::endl;
	}
};

// Add100 - Adds 100 to add5OutputParams and returns the result
class Add100 : public Callable<Add5OutputParams, Add100OutputParams> {
public:
	void invoke(const Add5OutputParams& input, Add100OutputParams& output) override
	{
		output.add100OutputParams = input.add5OutputParams + 100;
		std::cout << "ADDING_100" << std::endl;
	}
};

// Add20 - Adds 20 to add5OutputParams and returns the result
class Add20 : public Callable<Add5OutputParams, void> {
public:

	void invoke(const Add5OutputParams& input) override
	{
		std::cout << "Added_20: " << input.add5OutputParams + 20 << std::endl;
	}
};

// Subtract100 - Subtracts 100 from add100OutputParams and returns the result
class Subtract100 : public Callable<Add100OutputParams, void> {
public:
	void invoke(const Add100OutputParams& input) override
	{
		std::cout << "Subtracted_100: " << input.add100OutputParams - 100 << std::endl;
	}
};

// Subtract20 - Subtracts 20 from add100OutputParams and returns the result
class Subtract20 : public Callable<Add100OutputParams, void> {
public:

	void invoke(const Add100OutputParams& input) override
	{
		std::cout << "Subtracted_20: " << input.add100OutputParams - 20 << std::endl;
	}
};

int main()
{


	std::vector<glm::vec3> delaunayVertices;

	delaunayVertices.emplace_back(glm::vec3(5.38461, 2.49756, 0.0)); //0 - index 0
	delaunayVertices.emplace_back(glm::vec3(10.7969, -6.18571, 0.0)); //1 - index 1
	delaunayVertices.emplace_back(glm::vec3(3.59558, -3.68423, 0.0)); //2 - index 2
	delaunayVertices.emplace_back(glm::vec3(-2.01404, -8.78555, 0.0)); //4 - index 3
	delaunayVertices.emplace_back(glm::vec3(-6.17731, -5.06843, 0.0)); //5 - index 4
	delaunayVertices.emplace_back(glm::vec3(1.60165, -1.69653, 0.0)); //3 - index 5
	delaunayVertices.emplace_back(glm::vec3(5.38461, 2.49756, 0.0)); //0 - index 6


	//DELAUNAY_TEST
	//DelaunayTest::constrainedTest();
	//DelaunayTest::classicTest();

	Triangulator::triangulatePolygon(delaunayVertices);

	//dobre teraz by som mal otestovat moj novy triangulator



	//--INITIALIZATIONS_OF_FUNCTION_COMPOSERS--

	//--ADD_PLANE_COMPOSER--

	FunctionComposer addPlaneComposer;
	FunctionNode* addPlaneRoot = addPlaneComposer.initRoot<GenPlaneVertexDataCallable>();
	addPlaneComposer.addFunc<InitMeshVaoDataCallable>(addPlaneRoot);
	addPlaneComposer.addFunc<AddMeshIntoSceneCallable>(addPlaneRoot);
	AddPlaneCallback addPlaneCallBack(addPlaneComposer);

	//---FETCH_SURFACE_COMPOSER---
	FunctionComposer fetchSurfaceComposer;
	FunctionNode* fetchSurfaceRoot = fetchSurfaceComposer.initRoot<GenFetchedSurfaceVertexDataCallable>();
	fetchSurfaceComposer.addFunc<InitMeshVaoDataCallable>(fetchSurfaceRoot);
	fetchSurfaceComposer.addFunc<AddMeshIntoSceneCallable>(fetchSurfaceRoot);
	FetchSurfaceCallBack fetchSurfaceCallBack(fetchSurfaceComposer);


	FunctionComposer composer;
	auto rootNode = composer.initRoot<RootCreationSquare>();

	//FirstLevel
	auto add5Node = composer.addFunc<Add5>(rootNode);
	composer.addFunc<Divide10>(rootNode);
	composer.addFunc<Multiply10>(rootNode);
	composer.addFunc<Subtract10>(rootNode);

	//SecondLevel
	auto add100Node = composer.addFunc<Add100>(add5Node);
	composer.addFunc<Add20>(add5Node);

	//ThirdLevel
	composer.addFunc<Subtract100>(add100Node);
	composer.addFunc<Subtract20>(add100Node);

	RootInputParams rootInputParams;
	rootInputParams.rootInputValue = 2;

	//composer.execute(rootInputParams);
	//composer.execute();  
	//composer.execute(rootInputParams);
	composer.execute(rootInputParams);




	Application& app = Application::getInstance(SCR_WIDTH, SCR_HEIGHT, "SurfaceEditor");
	Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	Ray ray = Ray::getCameraRay(*camera);



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
	//scene.accessNeighbouringVertsAndFacesOfVertex(&mesh->m_halfEdgeStructure->m_vertices.at(0));


	//SelectionLayer toolBarLayer(SelectionLayer("SelectionLayer"));
	//app.getLayerStack().addLayer(&toolBarLayer);


	//viewPortLayer
	ViewPortLayer* viewPortLayer = new ViewPortLayer("viewPortLayer");
	viewPortLayer->m_activeCamera = camera;

	viewPortLayer->m_shaderSettings.m_faceShader = &meshShader;
	viewPortLayer->m_shaderSettings.m_meshShader = &meshShader;
	viewPortLayer->m_shaderSettings.m_normalShader = &normalsShader;
	viewPortLayer->m_shaderSettings.m_pointShader = &pointsShader;
	viewPortLayer->m_shaderSettings.m_edgeShader = &linesShader;

	ViewPortsHolder* viewPortsHolder = new ViewPortsHolder("viewPortsHolderLayer");
	viewPortsHolder->addViewPortLayer(viewPortLayer);
	viewPortsHolder->m_activeViewPortLayer = viewPortLayer;
	ObjectSelectionHolder* objectSelectionHolder = new ObjectSelectionHolder();

	viewPortsHolder->m_scene = &scene;
	ViewPortsHolderContext::m_viewPortsHolder = viewPortsHolder;
	ViewPortsHolderContext::m_objectSelectionHolder = objectSelectionHolder;
	ViewPortsHolderContext::m_camera = camera;
	ViewPortsHolderContext::m_window = app.m_window;


	AddPlaneParams planeProperties;
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

	scene.m_rendererData.aabbData.clearAABBData();



	for (auto& entry : scene.coordsOctreeMap) {
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

		camera->m_matrices.perspectiveMatrix = projection;
		camera->m_matrices.viewMatrix = view;

		ray = Ray::fromMousePos(*camera, projection, view, app.getWindow());

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


			
			for (auto& entry : scene.coordsOctreeMap) {
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

		
		for (auto& [mesh, materialVertexMap] : scene.m_rendererData.meshData.meshFacesVaoMap)
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
		for (auto& [mesh, linesVector] : scene.m_rendererData.meshData.meshLinesVaoMap)
		{
			linesShader.bind();
			Renderer::drawLines(linesVector);
			linesShader.unbind();
		}

		ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_edgeShader->bind();
		for (const auto& [region, vertices] : scene.m_rendererData.aabbData.vaoDataMap) {
			
			Renderer::drawBox(vertices);
		}
		ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_shaderSettings.m_edgeShader->unbind();

		app.run();
		
		app.getWindow().update();

		Input::resetFrameInput();
	}

	app.close();
	return 0;

}