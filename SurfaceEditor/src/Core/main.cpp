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



import Renderer.Shader;
import Renderer.Buffers;
import DataStructures;
//import LayerSystem.Layer.ImGuiLayer;
import LayerSystem;
import Scene;
//import Patterns.Observer;
import Geometry.Primitive.Cube;
import Camera;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

namespace glm
{
	// Custom comparison operator for glm::vec3
	inline bool operator<(const glm::vec3& lhs, const glm::vec3& rhs) {
		if (lhs.x < rhs.x) return true;
		if (lhs.x > rhs.x) return false;
		if (lhs.y < rhs.y) return true;
		if (lhs.y > rhs.y) return false;
		return lhs.z < rhs.z;
	}

	// Custom comparison operator for std::pair<glm::vec3, glm::vec3>
	inline bool operator<(const std::pair<glm::vec3, glm::vec3>& lhs, const std::pair<glm::vec3, glm::vec3>& rhs) {
		if (lhs.first < rhs.first) return true;
		if (rhs.first < lhs.first) return false;
		return lhs.second < rhs.second;
	}
}


Ray getCameraRay(glm::vec3& cameraPosition, glm::vec3& cameraFrontVector)
{
	return Ray(cameraPosition, cameraFrontVector);
}


void setRayLine(std::vector<LineVertex>& rayLine, Ray& ray)
{
	/*
	if(rayLine.empty())
	{
		rayLine.push_back(LineVertex(ray.origin));
		rayLine.push_back(LineVertex(ray.origin + (ray.direction * 150.0f)));
	} else
	{
		rayLine.at(0).position = ray.origin;
		rayLine.at(1).position = ray.origin + (ray.direction * 150.0f);
	}
	*/
}

Ray calculateMousePickingRay(Camera& camera, glm::mat4& perspectiveMat, glm::mat4& viewMat, Window& window)
{
	double mouseX, mouseY;
	glfwGetCursorPos(window.getWindowHandle(), &mouseX, &mouseY);

	//calculate Normalized Device Coordinates
	auto normalizedXCoord = (2.0 * mouseX) / window.getScreenWidth() - 1.0;
	auto normalizedYCoord = 1.0 - (2.0 * mouseY) / window.getScreenHeight();

	//calculate clip coordinates
	glm::vec4 clip = glm::vec4(normalizedXCoord, normalizedYCoord, -1.0, 1.0);

	//calculate camera space coordinates
	glm::mat4 inversePerspective = glm::inverse(perspectiveMat);
	glm::vec4 cameraSpace = inversePerspective * clip;
	cameraSpace[2] = -1.0;
	cameraSpace[3] = 0.0;

	//calculate view space coordinates
	glm::vec4 worldSpace = glm::inverse(viewMat) * cameraSpace;

	glm::vec3 rayPosition = glm::vec3(worldSpace);
	glm::vec3 normalizedRayPosition = glm::normalize(rayPosition);

	glm::vec3 rayStart = camera.position;



	//return the Ray object
	return Ray(rayStart, normalizedRayPosition);
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

//PROJECTION TEST

float calculateAngleBetweenVectors(const glm::vec3& a, const glm::vec3& b) {
	// Calculate the dot product of the two vectors
	float dotProduct = glm::dot(a, b);

	// Calculate the magnitudes of the vectors
	float magnitudeA = glm::length(a);
	float magnitudeB = glm::length(b);

	// Calculate the angle in radians using the dot product and magnitudes
	float angleRadians = std::acos(dotProduct / (magnitudeA * magnitudeB));

	// Convert radians to degrees
	float angleDegrees = glm::degrees(angleRadians);

	return angleDegrees;
}
/*
//prva vec co musime zistit je normalu a bod polygonu
void calculatePointAndNormalOfPolygon(const std::vector<int>& polygonIndices, const std::vector<glm::vec3>& vertices, glm::vec3& returnedNormal, glm::vec3& returnedPoint)
{
	glm::vec3 normal;
	//prvy point
	returnedPoint = vertices.at(polygonIndices.at(0));
	for (const auto& polygonVertexIndex : polygonIndices)
	{
		glm::vec3 vertex0 = vertices.at(polygonVertexIndex % polygonIndices.size());
		glm::vec3 vertex1 = vertices.at((polygonVertexIndex + 1) % polygonIndices.size());
		glm::vec3 vertex2 = vertices.at((polygonVertexIndex + 2) % polygonIndices.size());

		glm::vec3 vectorA = vertex2 - vertex1;
		glm::vec3 vectorB = vertex0 - vertex1;

		normal = glm::cross(vectorA, vectorB);

		// Check if the length of the normal vector is greater than zero
		if (glm::length(normal) > 0.0f)
		{
			// Normalize and assign the normal vector to the output parameter
			returnedNormal = glm::normalize(normal);
			return;
		}
	}

	// Return false if no valid normal is found
	return;
}
*/

void calculateOriginalVertexPositions(const glm::vec3 normalOfPolygon, const glm::vec3 pointOfPolygon, const std::vector<glm::vec3>& projectedPoints, std::vector<glm::vec3>& pointsAfterProjection)
{
	float movedBy = 0.0f;

	glm::vec3 vectorOfZPlane({ 0.0,0.0,1.0 });
	glm::vec3 reversedVectorOfZPlane = { 0.0, 0.0, -1.0 };
	for (const auto& projectedPoint : projectedPoints)
	{
		if (!RayIntersectionAlg::intersectPlane(normalOfPolygon, pointOfPolygon, projectedPoint, vectorOfZPlane, movedBy))
		{
			if(RayIntersectionAlg::intersectPlane(normalOfPolygon, pointOfPolygon, projectedPoint, reversedVectorOfZPlane, movedBy))
			{
				std::cout << "INT" << std::endl;
				std::cout << "movedBy: " << movedBy << std::endl;
				//pointsAfterProjection.push_back({ projectedPoint.x, projectedPoint.y, projectedPoint.z + () });
			} else
			{
				std::cout << "NOT INT" << std::endl;
			}
		} else
		{
			std::cout << "INT" << std::endl;
			std::cout << "movedBy: " << movedBy << std::endl;
		}
	}
	
}

float angleBetweenVectors(const glm::vec3& a, const glm::vec3& b) {
	// Calculate the dot product of the two vectors
	float dotProduct = glm::dot(a, b);

	// Calculate the magnitudes of the vectors
	float magnitudeA = glm::length(a);
	float magnitudeB = glm::length(b);

	// Calculate the angle in radians using the dot product and magnitudes
	float angleRadians = std::acos(dotProduct / (magnitudeA * magnitudeB));

	// Convert radians to degrees
	float angleDegrees = glm::degrees(angleRadians);

	return angleDegrees;
}


bool calculatePolygonNormal(const std::vector<int>& polygon, const PolygonOrientation& polygonOrientation, const std::vector<glm::vec3>& vertices, glm::vec3& returnedVector, glm::vec3& returnedPoint) {

	glm::vec3 normal;

	//prvy point
	returnedPoint = vertices.at(polygon.at(0));
	for (const auto& polygonVertexIndex : polygon)
	{
		glm::vec3 vertex0 = vertices.at(polygonVertexIndex % polygon.size());
		std::cout << "Vertex0: X = " << vertex0.x << ", Y = " << vertex0.y << ", Z = " << vertex0.z << std::endl;
		glm::vec3 vertex1 = vertices.at((polygonVertexIndex + 1) % polygon.size());
		std::cout << "Vertex1: X = " << vertex1.x << ", Y = " << vertex1.y << ", Z = " << vertex1.z << std::endl;
		glm::vec3 vertex2 = vertices.at((polygonVertexIndex + 2) % polygon.size());
		std::cout << "Vertex2: X = " << vertex2.x << ", Y = " << vertex2.y << ", Z = " << vertex2.z << std::endl;

		glm::vec3 vectorA = vertex2 - vertex1;
		std::cout << "VectorA: X = " << vectorA.x << ", Y = " << vectorA.y << ", Z = " << vectorA.z << std::endl;
		glm::vec3 vectorB = vertex0 - vertex1;
		std::cout << "VectorB: X = " << vectorB.x << ", Y = " << vectorB.y << ", Z = " << vectorB.z << std::endl;

		if (polygonOrientation == PolygonOrientation::COUNTERCLOCKWISE)
		{
			const auto& angle = angleBetweenVectors(vectorA, vectorB);
			std::cout << "angle: " << angle << std::endl;
			if (angle > 90.0)
			{
				normal = glm::cross(vectorB, vectorA);
			}
			else
			{
				normal = glm::cross(vectorA, vectorB);
			}
		}
		else
		{
			const auto& angle = angleBetweenVectors(vectorA, vectorB);
			std::cout << "angle: " << angle << std::endl;
			if (angle > 90.0)
			{
				normal = glm::cross(vectorA, vectorB);
			}
			else
			{
				normal = glm::cross(vectorB, vectorA);
			}
		}

		// Check if the length of the normal vector is greater than zero
		if (glm::length(normal) > 0.0f)
		{
			// Normalize and assign the normal vector to the output parameter
			returnedVector = glm::normalize(normal);
			std::cout << "ReturnedVector: X = " << returnedVector.x << ", Y = " << returnedVector.y << ", Z = " << returnedVector.z << std::endl;
			return true;
		}
	}

	// Return false if no valid normal is found
	return false;
}

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;



int main()
{
	//PROJECTION TEST

	//PROJECTION TEST


	//testMaterialov!
	
	Material* defaultMaterial = new Material("default");

	//dva materialy
	//blue
	Material* blueMaterial = new Material("blue");

	//red
	Material* redMaterial = new Material("red");

	//novy assignuty Material
	Material* material3 = new Material("3");

	//jeden mesh s 5 faceami


	std::vector<glm::vec3> meshVerts;

	std::vector<std::vector<int>> polygonsIndices;
	std::vector<PolygonOrientation> polygonOrientations;


	//firstPoly
	//meshVerts.push_back({ 5.38461 , 2.49756 , 0.0}); //0
	//meshVerts.push_back({ 10.7969 , -6.18571 , 0.0}); //1
	//meshVerts.push_back({ 3.59558 , -3.68423 , 0.0}); //2
	//meshVerts.push_back({ 1.60165 , -1.69653 , 0.0}); //3

	//secondPoly
	//meshVerts.push_back({ -2.01404 , -8.78555 , 0.0}); //4
	//meshVerts.push_back({ -6.17731 , -5.06843 , 0.0}); //5

	//thirdPoly
	//meshVerts.push_back({ -8.15201 , -2.79399 , 0.0}); //6
	//meshVerts.push_back({ -6.77999 , 0.257219 , 0.0}); //7
	//meshVerts.push_back({ -1.93392 , 2.98111 , 0.0}); //8


	//firstPoly
	//polygonOrientations.push_back(PolygonOrientation::CLOCKWISE);
	//polygonsIndices.push_back({ 3, 0, 1, 2 });
	//secondPoly
	//polygonOrientations.push_back(PolygonOrientation::CLOCKWISE);
	//polygonsIndices.push_back({ 3, 5, 4, 2 });
	//thirdPoly
	//polygonOrientations.push_back(PolygonOrientation::CLOCKWISE);
	//polygonsIndices.push_back({ 3, 8, 7, 6, 5 });

	
	meshVerts.push_back({ 5.0 , 5.0 , 5.0 }); //0
	meshVerts.push_back({ 10.0 , 5.0 , 5.0}); //1
	meshVerts.push_back({ 7.5 , 10.0 , 5.0 }); //2
	meshVerts.push_back({ 12.5, 10.0, 5.0 }); //3
	meshVerts.push_back({ 15.0, 5.0, 5.0 }); //4
	meshVerts.push_back({ 20.0, 5.0, 5.0}); //5
	meshVerts.push_back({ 17.5, 10.0, 5.0 }); //6
	meshVerts.push_back({ 15.0, 15.0, 5.0 }); //7

	polygonOrientations.push_back(PolygonOrientation::CLOCKWISE);
	polygonsIndices.push_back({ 0, 1, 2 });

	polygonOrientations.push_back(PolygonOrientation::CLOCKWISE);
	polygonsIndices.push_back({ 1, 3, 2 });

	polygonOrientations.push_back(PolygonOrientation::CLOCKWISE);
	polygonsIndices.push_back({ 1, 4, 3 });

	polygonOrientations.push_back(PolygonOrientation::CLOCKWISE);
	polygonsIndices.push_back({ 4, 6, 3 });

	polygonOrientations.push_back(PolygonOrientation::CLOCKWISE);
	polygonsIndices.push_back({ 4, 5, 6 });

	polygonOrientations.push_back(PolygonOrientation::CLOCKWISE);
	polygonsIndices.push_back({ 3, 6, 7 });




	//FIRST STEP
	Mesh* mesh = new Mesh(defaultMaterial, polygonsIndices, meshVerts);



	

	Application app(SCR_WIDTH, SCR_HEIGHT, "SurfaceEditor");
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Ray ray = getCameraRay(camera.position, camera.frontVector);
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

	Scene scene;


	
	//pridaj mesh
	//scene.addMesh2(mesh);

	//access
	//scene.accessNeighbouringVertsAndFacesOfVertex(&mesh->m_halfEdgeMesh->m_vertices.at(0));


	//ToolBarLayer toolBarLayer(ToolBarLayer("ToolBarLayer"));
	//app.onAddLayer(&toolBarLayer);
	

	//viewPortLayer
	ViewPortLayer* viewPortLayer = new ViewPortLayer("viewPortLayer", &camera, &scene);
	viewPortLayer->m_meshShader = &meshShader;
	viewPortLayer->m_normalsShader = &normalsShader;
	viewPortLayer->m_shaderSingleColor = &shaderSingleColor;
	viewPortLayer->m_pointsShader = &pointsShader;
	viewPortLayer->m_linesShader = &linesShader;
	viewPortLayer->m_shaderBlit = &shaderBlit;
	viewPortLayer->m_shaderDilation = &shaderDilation;
	app.onAddLayer(viewPortLayer);

	ViewPortHolder* viewPortHolder = new ViewPortHolder(viewPortLayer);
	PlaneProperties planeProperties;
	planeProperties.m_size = 50.0f;
	planeProperties.m_subdivisionLevel = 4;


	//ImporExportLayer
	ImportExportLayer* importExportLayer = new ImportExportLayer(viewPortHolder, "importExportLayer");
	app.onAddLayer(importExportLayer);

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
	app.onAddLayer(&toolBarLayer);

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
	app.onAddLayer(&additionLayer);

	//TransformLayer
	app.onAddLayer(&transformLayer);


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
		viewPortLayer->updateDeltaTime(deltaTime);
		//app.getWindow().processInput(camera, deltaTime);
		//Input::updateMousePosition();


		linesShader.bind();
		glm::mat4 projection = glm::mat4(1.0f);

		float nearPlane = 1.0f;
		float farPlane = 1000.0f;
		projection = glm::perspective(glm::radians(camera.zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), nearPlane, farPlane);
		linesShader.setMat4("u_projection", projection);

		glm::mat4 view = camera.lookAtMatrix;
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


		//ray = getCameraRay(camera.position, camera.frontVector);
		ray = calculateMousePickingRay(camera, projection, view, app.getWindow());
		camera.m_cameraRay = ray;


		if (Input::isKeyDown(GLFW_KEY_C))
		{
			basicSculptToolCommand.applyBrush(ray, 20.0f, 5.0f);

		}

		if (Input::isKeyDown(GLFW_KEY_V))
		{
			basicSculptToolCommand.applyBrush(ray, 20.0f, -5.0f);

		}

		if (Input::isKeyDown(GLFW_KEY_F))
		{
			/*
			OBJExporter objExporter;
			objExporter.open("C:\\Users\\Samuel\\Downloads\\Exports\\textFile.obj");
			int currentMeshIndex = 0;
			for (const auto selectedMesh : viewPortHolder->m_selectedMeshes)
			{
				selectedMesh->m_meshID = std::to_string(currentMeshIndex);
				objExporter.setMesh(selectedMesh);
				++currentMeshIndex;
			}
			objExporter.write();


			OBJImporter objImporter;
			objImporter.open("C:\\Users\\Samuel\\Downloads\\Exports\\surfaceExportTriangulated.obj");
			objImporter.read();
			//objImporter.getMeshes();
			
			for (const auto& importedMesh : objImporter.getMeshes())
			{
				ImportMeshCallBack importMeshCallBack(viewPortHolder, importedMesh);
				importMeshCallBack.execute();
			}
			*/
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


		viewPortHolder->renderMeshes();
		//viewPortHolder->render();


		app.run();

		
		app.getWindow().update();
	}

	app.close();
	return 0;

}