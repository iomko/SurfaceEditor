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

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//testing triangles for now
TriangleData createFirstTriangle()
{
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 position1, position2, position3;
	position1 = { -0.5f + 4.0f, -0.5f, 0.0f};
	position2 = { 0.5f + 4.0f, -0.5f, 0.0f };
	position3 = { 0.0f + 4.0f,0.5f, 0.0f};

	return TriangleData({ position1, position2, position3 }, color);
}

TriangleData createSecondTriangle()
{
	glm::vec3 color = glm::vec3(0.4f, 0.3f, 0.7f);

	glm::vec3 position1, position2, position3;
	position1 = { -0.5f + 1.0f, -0.5f, -1.5f };
	position2 = { 0.5f + 1.5f, -0.5f, 1.0f };
	position3 = { 0.0f + 2.0f,0.5f, 1.5f };

	return TriangleData({ position1, position2, position3 }, color);
}

int main()
{
	Application& app = Application::getInstance();
	app.run();
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glEnable(GL_DEPTH_TEST);

	Shader shader = Shader("src\\shader.vert", "src\\shader.frag");

	Mesh mesh;
	Mesh mesh2;

	Renderer renderer;
	renderer.init();

	std::vector<AABBBoundingRegion> bbs;


	AABBBoundingRegion bounds(glm::vec3(10.0f, 5.2f, -4.0f), glm::vec3(15.0f, 8.0f, 2.0f));

	AABBBoundingRegion testBounds(glm::vec3(12.0f, 6.0f, 1.0f), glm::vec3(13.0f, 7.0f, 0.0f));



	//test octree
	Octree<int, 1> octree(bounds.getMin(), bounds.getMax());
	octree.addDataToOctree(5, testBounds);

	AABBBoundingRegion testingCol(glm::vec3(8.0f, 9.0f, -4.0f), glm::vec3(11.0f, 5.0f, -1.0f));

	auto octreeBounds = octree.getAllBoundingBoxes();

	for (const AABBBoundingRegion& bb : octreeBounds)
	{
		renderer.collectAABBdata(bb, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	std::cout << "NumOfBounds:" << octreeBounds.size() << std::endl;

	//octree.addDataToOctree(6);

	/*
	auto t0 = octree.rootNode->nodeBounds;
	auto t1 = octree.rootNode->childrenNodes[0]->nodeBounds;
	auto t2 = octree.rootNode->childrenNodes[1]->nodeBounds;
	auto t3 = octree.rootNode->childrenNodes[2]->nodeBounds;
	auto t4= octree.rootNode->childrenNodes[3]->nodeBounds;
	auto t5 = octree.rootNode->childrenNodes[4]->nodeBounds;
	auto t6 = octree.rootNode->childrenNodes[5]->nodeBounds;
	auto t7 = octree.rootNode->childrenNodes[6]->nodeBounds;
	auto t8 = octree.rootNode->childrenNodes[7]->nodeBounds;
	*/

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


		if (Keyboard::keyWentDown(GLFW_KEY_Q) == true)
		{
			mesh.addTriangle(createFirstTriangle());
		}
		if (Keyboard::keyWentDown(GLFW_KEY_E) == true)
		{
			mesh.addTriangle(createSecondTriangle());
		}
		if(Keyboard::keyWentDown(GLFW_KEY_B) == true)
		{
			std::vector<glm::vec3> vecPositions;

			for (const auto& triangle : mesh.triangles)
			{
				for (const auto& position : triangle.positions) {
					vecPositions.push_back(position);
				}
			}
			AABBBoundingRegion bound(vecPositions);
			bbs.push_back(bound);
		}
		renderer.drawMesh(mesh);
		renderer.drawMesh(mesh2);


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