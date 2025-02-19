
/*
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
#include "ViewPortLayerRenderSettings.h"
#include "ViewPortLayerScreenSettings.h"
#include "../Core/Layer.h"
#include "../Scene/Camera.h"
#include "../ViewPortsHolderContext.h"
import Scene;

export class ViewPortLayer : public Layer
{
private:
	float m_deltaTime = 0.0f;
public:
	//Rendering
	ViewPortLayerRenderSettings m_shaderSettings;
	ViewPortLayerScreenSettings m_screenSettings;
	Camera* m_activeCamera = nullptr;
	
	//nechcem pre rozne viewPortLayere si uchovavat currentSelectedCommand zvlast
	//kedze vsetky budu pouzivat rovnaky currentSelectedCommand
	//Command* m_currentSelectedCommand = nullptr;
	//Params* m_currentSelectedCommandParams = nullptr;

	ViewPortLayer(const std::string& name)
		: Layer(name)
	{
		Renderer::init();

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		//zatial toto bude basic onEvent, tak ako keby som pracoval len s jedným viewPortLayerom zatial
		if (event.getType() == EventType::MouseButtonPress)
		{
			if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
			{
				if(ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand != nullptr)
				{
					if(ViewPortsHolderContext::m_viewPortsHolder->m_currentCommandParams != nullptr)
					{
						ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand->execute();
						event.isHandled = true;
					} else
					{
						ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand->execute(
							*ViewPortsHolderContext::m_viewPortsHolder->m_currentCommandParams);
						event.isHandled = true;
					}
				}
			}
		}

		if(event.getType() == EventType::MouseButtonPress || event.getType() == EventType::MouseMove)
		{
			updateCameraDirection(event);
		}
	}

};
*/