#include <iostream>
#include <vector>

#include "Shader.h"
#include "Window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Application.h"
#include "Mouse.h"
#include "Camera.h"
#include "Mesh.h"
#include "Octree.h"
#include "TriangleData.h"
#include "Renderer.h"
#include "TestLayer.h"
#include "ImGuiLayer.h"

#include "Input.h"


float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool drawBB = true;

//testing triangles for now
TriangleData createFirstTriangle()
{
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 myPositions[3] = {
	glm::vec3(-0.5f - 6.0f, -0.5f, -1.0f),
	glm::vec3(0.5f + 4.0f, -0.5f, -1.0f),
	glm::vec3(0.0f + 4.0f,0.5f, -1.0f)
	};

	return TriangleData(myPositions, color);
}

TriangleData createSecondTriangle()
{
	glm::vec3 color = glm::vec3(0.4f, 0.3f, 0.7f);

	glm::vec3 myPositions[3] = {
	glm::vec3(0.5f - 6.0f, -0.5f + 5.0f, -1.5f),
	glm::vec3(2.0f - 6.0f, -0.5f + 5.0f, 1.0f),
	glm::vec3(2.0f - 6.0f,0.5f + 5.0f, 1.5f)
	};


	return TriangleData(myPositions, color);
}

int main()
{
	Application app(800, 600, "SurfaceEditor");
	TestLayer* testLayer = new TestLayer();
	app.onAddLayer(testLayer);
	Camera camera = Camera(glm::vec3(0.0f, 0.0f,17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glEnable(GL_DEPTH_TEST);

	Shader shader = Shader("src\\shader.vert", "src\\shader.frag");

	Mesh mesh;
	Mesh mesh2;

	Renderer renderer;
	renderer.init();

	std::vector<AABBBoundingRegion> bbs;


	AABBBoundingRegion octreeBounds(glm::vec3(-8.0f, -6.0f, -5.0f), glm::vec3(8.0f, 6.0f, 5.0f));

	Octree<TriangleData, 1> octree(octreeBounds.getMin(), octreeBounds.getMax());

	glm::vec3 startPoint{};
	glm::vec3 endPoint{};

	while (!glfwWindowShouldClose(app.getWindow().windowHandle))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		app.getWindow().processInput(camera, deltaTime);
		app.getWindow().cleanUp();

		shader.bind();
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		glm::mat4 view = camera.lookAtMatrix;
		shader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		shader.setMat4("model", model);

		app.run();

		if (Keyboard::keyWentDown(GLFW_KEY_T)) {
			startPoint = camera.position;
			endPoint = camera.position + (camera.frontVector * 1000.0f);
			Ray r(camera.position, camera.frontVector);
			
			//find data in octree
			auto resultOctreeNode = octree.findDataInOctree(r);
			if(resultOctreeNode.first)
			{
				std::cout << "hit" << std::endl;
				octree.removeData(resultOctreeNode.first,resultOctreeNode.second);
				if(drawBB)
				{
					renderer.data.boundingBoxVertices.clear();
					auto itBegin = octree.begin();
					auto itEnd = octree.end();
					for (itBegin; itBegin != itEnd; ++itBegin) {
						renderer.collectAABBdata(itBegin->nodeBounds, glm::vec3(0.0f, 1.0f, 0.0f));
					}
				}
				std::cout << "amount of bounds generated: " << renderer.data.boundingBoxVertices.size() / 36 << std::endl;
			}


			
		}

		if (Keyboard::keyWentDown(GLFW_KEY_Q) == true)
		{
			TriangleData dataTriangle = createFirstTriangle();
			mesh.addTriangle(dataTriangle);
	
			AABBBoundingRegion bounds(dataTriangle.positions, 3);
		
			octree.addDataToOctree(dataTriangle, bounds);

			std::vector<OctreeNode<TriangleData>*> associatedValues;

			for (const auto& entry : octree.leafs) {
				if (entry.first == dataTriangle) {
					for (auto value : entry.second)
					{
						associatedValues.push_back(value);
					}
				}
			}

			if(drawBB)
			{
				renderer.data.boundingBoxVertices.clear();
				auto itBegin = octree.begin();
				auto itEnd = octree.end();
				for (itBegin; itBegin != itEnd; ++itBegin) {
					renderer.collectAABBdata(itBegin->nodeBounds, glm::vec3(0.0f, 1.0f, 0.0f));
				}
			}
		}
		if (Keyboard::keyWentDown(GLFW_KEY_E) == true)
		{
			TriangleData dataTriangle = createSecondTriangle();
			mesh.addTriangle(dataTriangle);
			AABBBoundingRegion bounds(dataTriangle.positions, 3);

			octree.addDataToOctree(dataTriangle, bounds);

			if(drawBB)
			{
				renderer.data.boundingBoxVertices.clear();
				auto itBegin = octree.begin();
				auto itEnd = octree.end();
				for (itBegin; itBegin != itEnd; ++itBegin) {
					renderer.collectAABBdata(itBegin->nodeBounds, glm::vec3(0.0f, 1.0f, 0.0f));
				}
			}
		}
		
		renderer.drawMesh(mesh);
		renderer.drawLine(startPoint, endPoint);
		if(drawBB)
		{
			renderer.drawBoundingBoxes();
		}
		shader.unbind();
		app.getWindow().createNewFrame();
	}
	app.close();
	return 0;
}