#include <iostream>
#include <vector>

#include "Shader.h"
#include "Window.h"
#include "VertexBufferObject.h"
#include "VertexArrayObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mouse.h"
#include "Camera.h"
#include "Mesh.h"
#include "TriangleData.h"
#include "BoundingRegion.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//testing triangles for now
TriangleData createFirstTriangle()
{
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 position1, position2, position3;
	position1 = { -0.5f, -0.5f, 1.0f };
	position2 = { 0.5f, -0.5f, -2.0f };
	position3 = { 0.0f,0.5f, 3.0f };

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
	Window window = Window(800, 600, "SurfaceEditor");
	window.initialize();
	window.setCallBackFunctions();
	glfwSetInputMode(window.windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glEnable(GL_DEPTH_TEST);

	Shader shader = Shader("src\\shader.vert", "src\\shader.frag");

	Mesh mesh;
	Mesh mesh2;
	

	while (!glfwWindowShouldClose(window.windowHandle))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.processInput(camera, deltaTime);
		window.cleanUp();

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

			AABB aabb = createAABB(vecPositions);

			glm::vec3 lineStart = aabb.min;
			glm::vec3 lineEnd = aabb.max;

			TriangleData triangleData({ lineStart, lineEnd, glm::vec3{lineStart.x - 0.1f,lineStart.y - 0.1f,lineStart.z - 0.1f} }, glm::vec3{ 0.0f,1.0f,0.0f });
			mesh2.addTriangle(triangleData);
		}

		mesh.render();
		mesh2.render();
		shader.unbind();

		window.createNewFrame();
	}
	window.terminate();
	return 0;
}