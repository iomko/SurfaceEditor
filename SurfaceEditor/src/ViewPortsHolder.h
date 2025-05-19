#pragma once
#include <vector>

#include "ObjectSelectionHolder.h"
#include "Patterns/Observer.h"
#include "Patterns/Command.h"
#include "Core/Layer.h"
#include "Scene/Scene.h"
#include "Scene/ViewPortLayerRenderSettings.h"
#include "Scene/ViewPortLayerScreenSettings.h"
#include "Tools/Tool.h"

class ViewPortLayer;

class ViewPortsHolder : public Layer, public Observer
{
public:
	ViewPortsHolder(const std::string& layerName): Layer(layerName)
	{
		Renderer::init();

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void addCommandToQueue(ICommand* command)
	{
		m_commandsQueue.push_back(command);
	}

	void addViewPortLayer(ViewPortLayer* viewPortLayer)
	{
		m_viewPortLayers.push_back(viewPortLayer);
	}

	std::vector<ViewPortLayer*> m_viewPortLayers;
	ViewPortLayer* m_activeViewPortLayer = nullptr;

	std::vector<ICommand*> m_commandsQueue;
	Scene* m_scene = nullptr;

	ITool* m_currentTool = nullptr;
	Params* m_currentToolParams = nullptr;

	//temporary
	int m_currentMeshId = 0;
};

class ViewPortsHolderContext
{
public:
	static inline ViewPortsHolder* m_viewPortsHolder = nullptr;
	static inline ObjectSelectionHolder* m_objectSelectionHolder = nullptr;
	static inline Camera* m_camera = nullptr;
	static inline Window* m_window = nullptr;
};

class ViewPortLayer : public Layer
{
private:
	float m_deltaTime = 0.0f;
public:
	//Rendering
	ViewPortLayerRenderSettings m_shaderSettings;
	ViewPortLayerScreenSettings m_screenSettings;
	Camera* m_activeCamera = nullptr;

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
		}
		if (Input::isKeyPressed(GLFW_KEY_S) == true)
		{
			m_activeCamera->updateCameraPosition(CameraMovement::BACKWARD);
		}
		if (Input::isKeyPressed(GLFW_KEY_A) == true)
		{
			m_activeCamera->updateCameraPosition(CameraMovement::LEFT);
		}
		if (Input::isKeyPressed(GLFW_KEY_D) == true)
		{
			m_activeCamera->updateCameraPosition(CameraMovement::RIGHT);
		}
	}


	void onEvent(Event& event) override
	{
		ViewPortsHolder* viewPortsHolder = ViewPortsHolderContext::m_viewPortsHolder;

		if (event.getType() == EventType::MouseButtonPress ||
			event.getType() == EventType::MouseScroll ||
			event.getType() == EventType::MouseButtonRelease ||
			event.getType() == EventType::MouseMove)
		{
			ITool* currentTool = viewPortsHolder->m_currentTool;
			Params* currentToolParams = viewPortsHolder->m_currentToolParams;
			if(currentTool != nullptr)
			{
				if (Input::isMouseButtonClicked(GLFW_MOUSE_BUTTON_LEFT))
				{
					std::cout << "Button_Clicked!" << std::endl;
					if (currentToolParams != nullptr)
					{
						currentTool->getInteractionHandler()->onBegin(*currentToolParams);
					}
					else
					{
						currentTool->getInteractionHandler()->onBegin();
					}
				}

				if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
				{
					if (currentToolParams != nullptr)
					{
						currentTool->getInteractionHandler()->onUpdate(*currentToolParams);
					}
					else
					{
						currentTool->getInteractionHandler()->onUpdate();
					}
				}
				event.isHandled = true;
				//event.isHandled = true; nepotrebujeme riesit isHandled kedze pojde o posledny Layer
			}

			updateCameraDirection(event);
		}
	}

};