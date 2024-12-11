module;
#include <unordered_set>

#include "../Core/Input.h"
#include "../Renderer/Renderer.h"
#include "../DataStructures/HalfEdge.h"
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"

//for now
//#include "../Mesh.h"

export module LayerSystem.Layer.ViewPortLayer;
#include "ViewPortShaderSettings.h"
#include "../Core/Layer.h"
#include "../Scene/Camera.h"
import Scene;

export class ViewPortLayer : public Layer
{
private:
	float m_deltaTime = 0.0f;

	Camera* m_activeCamera = nullptr;
	//vsetky viewporty budu vediet o jednej scene. //to znamena, ze akonahle nieco vymazem z jedneho viewportu, tak sa to
	//musi vymazat aj v tom druhom viewporte.
	Scene* m_scene = nullptr;

public:
	//Rendering
	ViewPortShaderSettings m_shaderSettings;

	//takto viewport musi vediet o svojej velkosti okna. Avsak nie o celkovej SCR_WIDTH a SCR_HEIGHT celeho glfw okna
	const unsigned int SCR_WIDTH = 1600;
	const unsigned int SCR_HEIGHT = 900;
	
	//nechcem pre rozne viewPortLayere si uchovavat currentSelectedCommand zvlast
	//kedze vsetky budu pouzivat rovnaky currentSelectedCommand
	Command* m_currentSelectedCommand = nullptr;

	ViewPortLayer(const std::string& name, Camera* m_active_camera, Scene* scene)
		: Layer(name), m_activeCamera(m_active_camera), m_scene(scene)
	{
		Renderer::init();

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Camera* getCamera()
	{
		return m_activeCamera;
	}

	Scene* getScene()
	{
		return m_scene;
	}

	void onUpdate() override
	{
		updateCameraMovement();
	}

	void updateCameraDirection(Event& event)
	{
		if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
		{
			m_activeCamera->updateCameraDirection(Input::getMouseDx(), Input::getMouseDy());
			event.isHandled = true;
		}
	}

	void updateCameraMovement()
	{
		const float movementSpeed = 50.0f * m_deltaTime;
		if (Input::isKeyPressed(GLFW_KEY_W) == true)
		{
			m_activeCamera->updateCameraPosition(CameraMovement::FORWARD);
			//std::cout << "camera moved forward" << std::endl;
		}
		if (Input::isKeyPressed(GLFW_KEY_S) == true)
		{
			m_activeCamera->updateCameraPosition(CameraMovement::BACKWARD);
			//std::cout << "camera moved backward" << std::endl;
		}
		if (Input::isKeyPressed(GLFW_KEY_A) == true)
		{
			m_activeCamera->updateCameraPosition(CameraMovement::LEFT);
			//std::cout << "camera moved left" << std::endl;
		}
		if (Input::isKeyPressed(GLFW_KEY_D) == true)
		{
			m_activeCamera->updateCameraPosition(CameraMovement::RIGHT);
			//std::cout << "camera moved right" << std::endl;
		}
	}


	void onEvent(Event& event) override
	{
		if (event.getType() == EventType::MouseButtonPress)
		{
			
			if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
			{
				if (m_currentSelectedCommand != nullptr)
				{
					m_currentSelectedCommand->execute();
				}
			}
		}

		if(event.getType() == EventType::MouseButtonPress || event.getType() == EventType::MouseMove)
		{
			updateCameraDirection(event);
		}
	}

};
