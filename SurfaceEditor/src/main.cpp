#include <iostream>
#include <vector>

#include "Shader.h"
#include "Window.h"
#include "VertexBufferObject.h"
#include "VertexArrayObject.h"

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

#include "TestClass.h"


float deltaTime = 0.0f;
float lastFrame = 0.0f;

//testing triangles for now
TriangleData createFirstTriangle()
{
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

	/*
	glm::vec3 position1, position2, position3;
	position1 = { -0.5f -6.0f, -0.5f, 0.0f};
	position2 = { 0.5f + 4.0f, -0.5f, 0.0f };
	position3 = { 0.0f + 4.0f,0.5f, 0.0f};
	*/

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

	/*
	glm::vec3 position1, position2, position3;
	position1 = { 0.5f - 6.0f, -0.5f + 5.0f, -1.5f };
	position2 = { 2.0f - 6.0f, -0.5f + 5.0f, 1.0f };
	position3 = { 2.0f - 6.0f,0.5f + 5.0f, 1.5f };
	*/

	glm::vec3 myPositions[3] = {
	glm::vec3(0.5f - 6.0f, -0.5f + 5.0f, -1.5f),
	glm::vec3(2.0f - 6.0f, -0.5f + 5.0f, 1.0f),
	glm::vec3(2.0f - 6.0f,0.5f + 5.0f, 1.5f)
	};


	return TriangleData(myPositions, color);
}

int main()
{
	Application& app = Application::getInstance();
	app.run();
	Camera camera = Camera(glm::vec3(0.0f, 0.0f,17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glEnable(GL_DEPTH_TEST);

	Shader shader = Shader("src\\shader.vert", "src\\shader.frag");

	Mesh mesh;
	Mesh mesh2;

	Renderer renderer;
	renderer.init();

	std::vector<AABBBoundingRegion> bbs;


	AABBBoundingRegion bounds(glm::vec3(-8.0f, -6.0f, -5.0f), glm::vec3(8.0f, 6.0f, 5.0f));

	AABBBoundingRegion testBounds(glm::vec3(12.0f, 6.0f, 1.0f), glm::vec3(13.0f, 7.0f, 0.0f));
	
	AABBBoundingRegion testBounds2(glm::vec3(10.0f, 6.0f, -3.0f), glm::vec3(11.0f, 7.0f, -4.0f));
	
	//test octree
	Octree<TriangleData, 1> octree(bounds.getMin(), bounds.getMax());
	//octree.addDataToOctree(6, testBounds);
	//octree.addDataToOctree(7, testBounds2);
	//iteration test
	auto itBegin = octree.begin();
	auto itEnd = octree.end();


	for (itBegin; itBegin != itEnd; ++itBegin) {
		if (!itBegin->nodeData.empty()) {
			for (auto data : itBegin->nodeData)
			{
				//std::cout <<"data: " << data << std::endl;
			}
		}
	}
	
	

	AABBBoundingRegion testingCol(glm::vec3(8.0f, 9.0f, -4.0f), glm::vec3(11.0f, 5.0f, -1.0f));

	//auto octreeBounds = octree.getAllBoundingBoxes();

	/*
	for (const AABBBoundingRegion& bb : octreeBounds)
	{
		renderer.collectAABBdata(bb, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	*/
	//renderer.collectAABBdata(testBounds, glm::vec3(1.0f, 0.0f, 0.0f));
	//renderer.collectAABBdata(testBounds2, glm::vec3(0.0f, 0.0f, 1.0f));

	//std::cout << "NumOfBounds:" << octreeBounds.size() << std::endl;

	//LINES
	glm::vec3 startPoint{0.0f, 0.0f, 0.0f};
	glm::vec3 endPoint{ 0.0f, 0.0f, 0.0f };



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

		if (Keyboard::keyWentDown(GLFW_KEY_T)) {
			startPoint = camera.position;
			endPoint = camera.position + (camera.frontVector * 1000.0f);
			Ray r(camera.position, camera.frontVector);
			if (testBounds.intersectsRay(r)) {
				std::cout << "Ray intersects" << std::endl;
			}
			
			//find data in octree
			auto resultOctreeNode = octree.findDataInOctree(r);
			if(resultOctreeNode.first)
			{

				std::cout << "hit" << std::endl;
				octree.removeData(resultOctreeNode.first,resultOctreeNode.second);
				renderer.data.boundingBoxVertices.clear();
				auto itBegin = octree.begin();
				auto itEnd = octree.end();
				for (itBegin; itBegin != itEnd; ++itBegin) {
					renderer.collectAABBdata(itBegin->nodeBounds, glm::vec3(0.0f, 1.0f, 0.0f));
				}
			}


			std::cout << "amount of bounds generated: " << renderer.data.boundingBoxVertices.size()/36 << std::endl;
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

			//auto octreeBounds = octree.getAllBoundingBoxes();
			//renderer.collectAABBdata(bb, glm::vec3(0.0f, 1.0f, 0.0f));

			renderer.data.boundingBoxVertices.clear();
			auto itBegin = octree.begin();
			auto itEnd = octree.end();
			for (itBegin; itBegin != itEnd; ++itBegin) {
				renderer.collectAABBdata(itBegin->nodeBounds, glm::vec3(0.0f, 1.0f, 0.0f));
			}

		}
		if (Keyboard::keyWentDown(GLFW_KEY_E) == true)
		{
			TriangleData dataTriangle = createSecondTriangle();
			mesh2.addTriangle(dataTriangle);
			AABBBoundingRegion bounds(dataTriangle.positions, 3);

			octree.addDataToOctree(dataTriangle, bounds);

			//auto octreeBounds = octree.getAllBoundingBoxes();
			//renderer.collectAABBdata(bb, glm::vec3(0.0f, 1.0f, 0.0f));
			renderer.data.boundingBoxVertices.clear();
			auto itBegin = octree.begin();
			auto itEnd = octree.end();
			for (itBegin; itBegin != itEnd; ++itBegin) {
				renderer.collectAABBdata(itBegin->nodeBounds, glm::vec3(0.0f, 1.0f, 0.0f));
			}
		}
		if(Keyboard::keyWentDown(GLFW_KEY_B) == true)
		{
			/*
			std::vector<glm::vec3> vecPositions;

			for (const auto& triangle : mesh.triangles)
			{
				for (const auto& position : triangle.positions) {
					vecPositions.push_back(position);
				}
			}
			AABBBoundingRegion bound(vecPositions);
			bbs.push_back(bound);
			*/
		}
		renderer.drawMesh(mesh);
		renderer.drawMesh(mesh2);
		renderer.drawLine(startPoint, endPoint);

		//renderer.collectAABBdata(testBounds, glm::vec3(1.0f, 0.0f, 0.0f));
		//renderer.collectAABBdata(testingCol, glm::vec3(0.0f, 0.0f, 1.0f));
		
		renderer.drawBoundingBoxes();
		
		//renderer.collectAABBdata(testBounds, glm::vec3(1.0f, 0.0f, 0.0f));
		
		/*
		for (const AABBBoundingRegion& bb : bbs)
		{
			//renderer.collectAABBdata(bb, glm::vec3{0.0f, 1.0f, 0.0f});
			
			renderer.collectAABBdata(t0, glm::vec3{ 0.0f, 1.0f, 0.0f });
			renderer.collectAABBdata(t1, glm::vec3{ 0.0f, 1.0f, 0.0f });
			renderer.collectAABBdata(t2, glm::vec3{ 0.0f, 1.0f, 0.0f });
			renderer.collectAABBdata(t3, glm::vec3{ 0.0f, 1.0f, 0.0f });
			renderer.collectAABBdata(t4, glm::vec3{ 0.0f, 1.0f, 0.0f });
			renderer.collectAABBdata(t5, glm::vec3{ 0.0f, 1.0f, 0.0f });
			renderer.collectAABBdata(t6, glm::vec3{ 0.0f, 1.0f, 0.0f });
			renderer.collectAABBdata(t7, glm::vec3{ 0.0f, 1.0f, 0.0f });
			renderer.collectAABBdata(t8, glm::vec3{ 0.0f, 1.0f, 0.0f });
			renderer.collectAABBdata(testBounds, glm::vec3{ 1.0f, 0.0f, 0.0f });
			
			
		}
		*/

		

		shader.unbind();

		app.getWindow().createNewFrame();
	}
	app.close();
	return 0;
}