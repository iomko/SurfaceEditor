#include <iostream>

#include "Shader.h"
#include "Window.h"
#include "VertexBufferObject.h"
#include "VertexArrayObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mouse.h"
#include "Camera.h"


void processKeys(Camera& camera, float& deltaTime)
{
	const float movementSpeed = 2.5f * deltaTime;
	if (Keyboard::key(GLFW_KEY_W) == true)
	{
		camera.updateCameraPosition(CameraMovement::FORWARD, movementSpeed);
	}
	if (Keyboard::key(GLFW_KEY_S) == true)
	{
		camera.updateCameraPosition(CameraMovement::BACKWARD, movementSpeed);
	}
	if (Keyboard::key(GLFW_KEY_A) == true)
	{
		camera.updateCameraPosition(CameraMovement::LEFT, movementSpeed);
	}
	if (Keyboard::key(GLFW_KEY_D) == true)
	{
		camera.updateCameraPosition(CameraMovement::RIGHT, movementSpeed);
	}
}

void processLookAtDirection(Camera& camera)
{
	double changedDX = Mouse::getDiffX();
	double changedDY = Mouse::getDiffY();

	if (changedDX != 0.0 || changedDY != 0.0)
	{
		camera.updateCameraDirection(changedDX, changedDY);
	}
}


void processScroll(Camera& camera)
{
	double changedScroll = Mouse::getScrollDiffY();
	if(changedScroll != 0.0)
	{
		camera.updateCameraZoom(changedScroll);
	}
}

void processInput(Camera& camera, float& deltaTime)
{
	processKeys(camera, deltaTime);
	processLookAtDirection(camera);
	processScroll(camera);
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	Window window = Window(800, 600, "SurfaceEditor");
	window.initialize();
	window.setCallBackFunctions();
	glfwSetInputMode(window.windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glEnable(GL_DEPTH_TEST);

	Shader shader = Shader("src\\shader.vert", "src\\shader.frag");

	float vertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f
	};


	VertexArrayObject vao = VertexArrayObject();
	VertexBufferObject vbo = VertexBufferObject(vertices, sizeof(vertices), GL_STATIC_DRAW);
	vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	vao.addVertexBufferLayout(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	vbo.unbind();
	vao.unbind();

	while (!glfwWindowShouldClose(window.windowHandle))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(camera, deltaTime);

		window.cleanUp();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		shader.bind();
		glm::mat4 view = camera.lookAtMatrix;
		shader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		shader.setMat4("model", model);
		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		window.createNewFrame();
	}
	window.terminate();
	return 0;
}

