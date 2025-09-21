#include "Callbacks/CreatePrintStructureCallBack.h"
#include "Commands/CreatePrintCommand.h"
#include "UI/ModifiersLayer.h"
#include "UI/PrintableMeshSettingsPopUpLayer.h"
#define NOMINMAX  // Prevents Windows.h from defining min/max macros

#include <limits>
//#include <Windows.h>
#include <print>
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
#include "../DataStructures/Octree.h"
#include <tiffio.h>
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

#include "../DataStructures/ExtendedHalfEdge.h"

#include "../Utils/GeometryUtils.h"

#include "UI/OutlinerLayer.h"
#include "Callables/MeshOutlinerAdderCallable.h"
#include "DataStructures/PrintableMesh.h"

//INTERACTION_HANDLER
#include "../Tools/InteractionHandler.h"
#include "../Tools/Tool.h"

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;



//=====TESTING THE FUNCTION_COMPOSER=====


//=====PLANE_MESH_DATA=====

//OUTPUT

std::string getShaderPath(const std::string& file){
    return std::string(SHADER_DIR) + "/" + file;
}

/*
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
*/


void DebugDrawSimpleLine(Shader* debugShader) {
    struct P { float x,y,z; float h; };
    P pts[] = { {-0.5f,-0.5f,0,0}, {0.5f,0.5f,0,0} };
    GLuint tmpVAO=0, tmpVBO=0;
    glGenVertexArrays(1, &tmpVAO);
    glGenBuffers(1, &tmpVBO);

    glBindVertexArray(tmpVAO);
    glBindBuffer(GL_ARRAY_BUFFER, tmpVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(P), (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(P), (void*)(offsetof(P,h))); glEnableVertexAttribArray(1);

    debugShader->bind();
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(tmpVAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
    debugShader->unbind();

    glDeleteBuffers(1, &tmpVBO);
    glDeleteVertexArrays(1, &tmpVAO);
}

struct OutlinerDataTest {
    std::string someData = "same";
};



int main()
{
    /*
    PrintableMesh printableMesh;
    std::vector<ExtrudeEdge> extrudeEdges;
    extrudeEdges.emplace_back( glm::vec3{-20.0f, 0.0f, -20.0f },glm::vec3{-5.0f, 0.0f, -20.0f} ); //1
    extrudeEdges.emplace_back( glm::vec3{-5.0f, 0.0f, -20.0f },glm::vec3{5.0f, 0.0f, -20.0f} ); //2
    extrudeEdges.emplace_back( glm::vec3{-4.0f, 0.0f, 20.0f },glm::vec3{-20.0f, 0.0f, 20.0f} ); //3
    extrudeEdges.emplace_back( glm::vec3{-20.0f, 0.0f, -8.0f },glm::vec3{-20.0f, 0.0f, 10.0f} ); //4
    extrudeEdges.emplace_back( glm::vec3{-20.0f, 0.0f, 18.0f },glm::vec3{-20.0f, 0.0f, 13.0f} ); //5
    extrudeEdges.emplace_back( glm::vec3{-20.0f, 0.0f, -15.0f },glm::vec3{-20.0f, 0.0f, -11.0f} ); //6
    extrudeEdges.emplace_back( glm::vec3{-20.0f, 0.0f, -20.0f },glm::vec3{-20.0f, 0.0f, -15.0f} ); //7
    extrudeEdges.emplace_back( glm::vec3{5.0f, 0.0f, -8.0f },glm::vec3{2.0f, 0.0f, 20.0f} ); //8
    extrudeEdges.emplace_back( glm::vec3{-20.0f, 0.0f, -8.0f },glm::vec3{-20.0f, 0.0f, -11.0f} ); //9
    extrudeEdges.emplace_back( glm::vec3{-20.0f, 0.0f, 20.0f },glm::vec3{-20.0f, 0.0f, 18.0f} ); //10
    extrudeEdges.emplace_back( glm::vec3{5.0f, 0.0f, -8.0f },glm::vec3{5.0f, 0.0f, -20.0f} ); //11
    extrudeEdges.emplace_back( glm::vec3{-20.0f, 0.0f, 10.0f },glm::vec3{-20.0f, 0.0f, 13.0f} ); //12
    extrudeEdges.emplace_back( glm::vec3{-4.0f, 0.0f, 20.0f },glm::vec3{2.0f, 0.0f, 20.0f} ); //13
   
    printableMesh.addLevelLayers(extrudeEdges);
    */
    //printableMesh.addLevelLayers(extrudeEdges);
    
    WindowLayerBus windowLayerBus;

    ExtendedHalfEdgeMesh extendedHalfEdgeMeshTest;
    std::vector<glm::vec3> testVertices;
    testVertices.emplace_back(-6.17731, -5.06843, 0.0);
    testVertices.emplace_back(-8.15201, -2.79399, 0.0);
    testVertices.emplace_back(-6.77999, 0.257219, 0.0);
    testVertices.emplace_back(-1.93392, 2.98111, 0.0);
    testVertices.emplace_back(1.60165, -1.69653, 0.0);
    extendedHalfEdgeMeshTest.addFloatingFace(testVertices);

    //intersection test

    std::vector<glm::vec2> selectedFace2DVertices;
    selectedFace2DVertices.emplace_back(374, 372);
    selectedFace2DVertices.emplace_back(374, 374);
    selectedFace2DVertices.emplace_back(376, 374);

    std::vector<glm::vec2> face2DVertices;
    face2DVertices.emplace_back(374, 372);
    face2DVertices.emplace_back(376, 374);
    face2DVertices.emplace_back(376, 372);

    bool testOverlap = utils::geometry::polygon2DOverlap(selectedFace2DVertices, face2DVertices);


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

	Ray ray = Ray::getCameraRay(*camera);


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
	ViewPortsHolderContext::s_window = app.m_window;


	PlaneParams planeProperties;
	planeProperties.m_size = 50.0f;
	planeProperties.m_subdivisionLevel = 4;

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

	BrushToolCallBack brushToolCallBack;
	commandRegistry->registerCommand<BrushToolCommand>();
	BrushToolCommand* brushToolCommand = commandRegistry->getCommand<BrushToolCommand>();

	brushToolCommand->addObserver(&brushToolCallBack);
	brushToolCallBack.observe(brushToolCommand, &brushToolCallBack);

	//ToolRegistry::registerTool<BrushTool>(brushToolCommand, new BrushInteractionHandler());
	ToolRegistry::registerTool<BrushTool>(brushToolCommand);

	DeselectMeshCallBack deselectMeshCallBack;
	commandRegistry->registerCommand<DeselectMeshCommand>();
	DeselectMeshCommand* deselectMeshCommand = commandRegistry->getCommand<DeselectMeshCommand>();

	deselectMeshCommand->addObserver(&deselectMeshCallBack);
	deselectMeshCallBack.observe(deselectMeshCommand, &deselectMeshCallBack);

	SelectFaceCallBack selectFaceCallBack;
	commandRegistry->registerCommand<SelectFaceCommand>();
	SelectFaceCommand* selectFaceCommand = commandRegistry->getCommand<SelectFaceCommand>();
	selectFaceCommand->addObserver(&selectFaceCallBack);
	selectFaceCallBack.observe(selectFaceCommand, &selectFaceCallBack);

	ToolRegistry::registerTool<FaceSelectionTool>(selectFaceCommand);

	DeselectFaceCallBack deselectFaceCallBack;
	commandRegistry->registerCommand<DeselectFaceCommand>();
	DeselectFaceCommand* deselectFaceCommand = commandRegistry->getCommand<DeselectFaceCommand>();
	deselectFaceCommand->addObserver(&deselectFaceCallBack);
	deselectFaceCallBack.observe(deselectFaceCommand, &deselectFaceCallBack);

	ToolRegistry::registerTool<FaceDeselectionTool>(deselectFaceCommand);

	DeleteSelectedFacesCallBack deleteSelectedFacesCallBack;
	commandRegistry->registerCommand<DeleteSelectedFacesCommand>();
	DeleteSelectedFacesCommand* deleteSelectedFacesCommand = commandRegistry->getCommand<DeleteSelectedFacesCommand>();
	deleteSelectedFacesCommand->addObserver(&deleteSelectedFacesCallBack);
	deleteSelectedFacesCallBack.observe(deleteSelectedFacesCommand, &deleteSelectedFacesCallBack);

	DeleteSelectedMeshesCallBack deleteSelectedMeshesCallBack;
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
	AdditionLayer additionLayer("AdditionLayer", *commandRegistry);
	app.getLayerStack().addLayer(&additionLayer);

    //OutlinerLayer
    OutlinerLayer outlinerLayer("OutlinerLayer", windowLayerBus);
    app.getLayerStack().addLayer(&outlinerLayer);

    //popUpLayer
    PrintableMeshSettingsPopUpLayer popUpLayer("PopUpLayer", *commandRegistry, windowLayerBus);
    app.getLayerStack().addLayer(&popUpLayer);

    /*
    OutlinerNodeConcept* mesh1Concept = outlinerLayer.addNode(0, "Mesh1", OutlinerDataTest{});
    OutlinerNodeConcept* material1Concept = outlinerLayer.addChildNode(mesh1Concept, 1, "Material1", OutlinerDataTest{});
    outlinerLayer.addChildNode(material1Concept, 2, "Texture1", OutlinerDataTest{});
    outlinerLayer.addChildNode(material1Concept, 3, "Texture2", OutlinerDataTest{});
    outlinerLayer.addChildNode(material1Concept, 4, "Texture3", OutlinerDataTest{});
    outlinerLayer.addChildNode(mesh1Concept, 5, "PrintableMesh1", OutlinerDataTest{});
    outlinerLayer.addChildNode(mesh1Concept, 6, "PrintableMesh2", OutlinerDataTest{});
    outlinerLayer.addNode(7, "Mesh2", OutlinerDataTest{});
    OutlinerNodeConcept* material3Concept = outlinerLayer.addNode(8, "Mesh3", OutlinerDataTest{});
    outlinerLayer.addChildNode(material3Concept, 9, "Material2", OutlinerDataTest{});
    */

	//ModifiersLayer
	ModifiersLayer modifiersLayer("ModifiersLayer", *commandRegistry, windowLayerBus);
	app.getLayerStack().addLayer(&modifiersLayer);

	//RemovalLayer
	RemovalLayer removalLayer("RemovalLayer", *commandRegistry);
	app.getLayerStack().addLayer(&removalLayer);

	//SculptToolsLayer
	SculptToolsLayer sculptToolsLayer("SculptToolsLayer");
	app.getLayerStack().addLayer(&sculptToolsLayer);

	//clearAABBData();
    /*
	for (auto& entry : scene.m_res.coordsOctreeMap) {
		const glm::vec3& key = entry.first;
		Octree<std::pair<Mesh*, ExtendedFace*>>& octree = entry.second;
		for (auto& octreeNode : octree) {
			octreeNode.getBounds();
			collectAABBData(octreeNode.getBounds());
		}
	}
    */

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

    
    //test for edge connections
    //
    /*
    std::vector<glm::vec3> connectionVertices;
    connectionVertices.emplace_back(-1.9639, 0.0, -0.551); //0
    connectionVertices.emplace_back(-1.0, 0.0, -1.0); //1
    connectionVertices.emplace_back(1.0, 0.0, -1.0); //2
    connectionVertices.emplace_back(2.0096, 0.0, 0.37151); //3
    connectionVertices.emplace_back(1.0, 0.0, 1.0); //4
    connectionVertices.emplace_back(-1.0, 0.0, 1.0); //5
    connectionVertices.emplace_back(-1.0, 0.0, 2.7877);
    std::vector<std::vector<int>> connectionPolygons;
    
    std::vector<int> firstPolygon { 0, 5, 1 };
    std::vector<int> secondPolygon { 1, 5, 2 };
    std::vector<int> thirdPolygon { 2, 5, 4 };
    std::vector<int> fourthPolygon { 2, 4, 3 };

    connectionPolygons.push_back(firstPolygon);
    connectionPolygons.push_back(secondPolygon);
    connectionPolygons.push_back(thirdPolygon);
    connectionPolygons.push_back(fourthPolygon);

    ExtendedHalfEdgeMesh connectionHalfEdgeMesh;
    connectionHalfEdgeMesh.build(connectionPolygons, connectionVertices);

    //addFloatingFace
    //
    ExtendedEdge* firstEdge = nullptr;

    for(ExtendedEdge* extendedEdge : connectionHalfEdgeMesh.m_edges) {
        if((extendedEdge->m_firstVertex->m_position == connectionVertices.at(5)
            && extendedEdge->m_secondVertex->m_position == connectionVertices.at(4)) 
            || (extendedEdge->m_secondVertex->m_position == connectionVertices.at(5) 
            && extendedEdge->m_firstVertex->m_position == connectionVertices.at(4))) 
        {
            firstEdge = extendedEdge;
            break;
        }
    }

    std::vector<glm::vec3> floatingFace { { -1.0, 0.0, 1.0 }, { -1.0, 0.0, 2.7877 }, { 1.0, 0.0, 1.0 } };
    ExtendedEdge* secondEdge = nullptr;
    ExtendedFace* floatingExtendedFace = connectionHalfEdgeMesh.addFloatingFace(floatingFace);
    for(auto it = floatingExtendedFace->faceHalfEdgeBegin(); it != floatingExtendedFace->faceHalfEdgeEnd(); ++it) {
        ExtendedEdge* floatingEdge = (*it).m_edge;
        if((floatingEdge->m_firstVertex->m_position == connectionVertices.at(5)
            && floatingEdge->m_secondVertex->m_position == connectionVertices.at(4)) 
            || (floatingEdge->m_secondVertex->m_position == connectionVertices.at(5) 
            && floatingEdge->m_firstVertex->m_position == connectionVertices.at(4))) 
        {
            secondEdge = floatingEdge;
            break;
        }
    }

    connectionHalfEdgeMesh.connectEdges(firstEdge, secondEdge);
    */

    //connectionHalfEdgeMesh.connectEdges(ExtendedEdge *firstEdge, ExtendedEdge *secondEdge)


    //unsigned int tmpVAO=0, tmpVBO=0;
    //glGenVertexArrays(1, &tmpVAO); 
    //glGenBuffers(1, &tmpVBO);
    //
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

		if (Input::isKeyPressed(GLFW_KEY_C))
		{
 			basicSculptToolCommand.applyBrush(ray, 20.0f, 5.0f);
		}

		if (Input::isKeyPressed(GLFW_KEY_V))
		{
			basicSculptToolCommand.applyBrush(ray, 20.0f, -5.0f);

		}

		int screenWidth = ViewPortsHolderContext::s_window->getScreenWidth();
		int screenHeight = ViewPortsHolderContext::s_window->getScreenHeight();
		glViewport(0, 0, screenWidth, screenHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //chcem zapisovat do depth bufferu
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        //nechcem zapisovat do stencil bufferu
        glStencilMask(0x00);
        glDisable(GL_STENCIL_TEST);

        //DebugDrawSimpleLine(&linesShader);

        for (auto& [mesh, _] : scene.m_res.meshFaceOctreeCoordsMap) {
            /*
            if(scene.m_res.printableMeshMap.empty()) {
                Renderer::drawMesh(mesh);
                Renderer::drawMeshLines(mesh, &linesShader);
            }
            */

            Renderer::drawMesh(mesh);
            Renderer::drawMeshLines(mesh, &linesShader);
        }

        for(auto& [_, printableMesh] : scene.m_res.printableMeshMap) {
            Renderer::drawPrintableMesh(printableMesh, &linesShader);
        }

		//ViewPortsHolderContext::s_viewPortsController->m_viewPortLayers.at(0)->m_shaderSettings.m_edgeShader->bind();
	    
        /*
		for (const auto& [region, vertices] : Renderer::s_stageData.aabbVertsMap) {
			
			Renderer::drawBox(vertices);
		}
        */

		//ViewPortsHolderContext::s_viewPortsController->m_viewPortLayers.at(0)->m_shaderSettings.m_edgeShader->unbind();

		app.run();
		
		app.getWindow().update();

		Input::resetFrameInput();
	}

	app.close();
	return 0;

}
