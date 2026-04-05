#pragma once
#include <vector>

#include "Editing/Selection/SelectionController.h"
#include "Patterns/Observer.h"
#include "Scene/ViewPortLayerRenderSettings.h"
#include "Tools/Tool.h"
#include "Core/Input.h"
//#include "Renderer/Renderer.h"

class ViewPortLayer;

class ViewPortsController : public Observer
{
public:
	ViewPortsController() = default;

	void addLayer(ViewPortLayer* viewPortLayer)
	{
		m_viewPortLayers.push_back(viewPortLayer);
	}

	std::vector<ViewPortLayer*> m_viewPortLayers;
	ViewPortLayer* m_activeViewPortLayer = nullptr;

	std::vector<CommandConcept*> m_commandsQueue;
	Scene* m_scene = nullptr;

	ITool* m_currentTool = nullptr;
	OpParams* m_currentToolParams = nullptr;
};

class ViewPortsUILayerController
{
public:
	void registerUiWindow(OverlappingWindow* uiWindow)
	{
		m_uiWindow.push_back(uiWindow);
	}

	bool clickedOnUiWindow(const glm::vec2& clickPos)
	{
		for (auto& window : m_uiWindow)
		{
			if (window->clickedOnWindow(clickPos))
			{
				return true;
			}
		}

		return false;
	}

private:
	std::vector<OverlappingWindow*> m_uiWindow;
};

class ViewPortsHolderContext
{
public:
	static inline Window* s_window 								  				  = nullptr;
	static inline std::unique_ptr<ViewPortsUILayerController> s_uiLayerController = std::make_unique<ViewPortsUILayerController>();
	static inline std::unique_ptr<ViewPortsController> s_viewPortsController 	  = std::make_unique<ViewPortsController>();
	static inline std::unique_ptr<SelectionController> s_selectionController 	  = std::make_unique<SelectionController>();
	static inline std::unique_ptr<Camera> s_camera =
		std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
};

class ViewPortLayer : public Layer, public Observable
{
private:
	float m_deltaTime = 0.0f;
public:
	ViewPortLayerRenderSettings m_shaderSettings;
	Camera* m_camera = nullptr;

	ViewPortLayer(const std::string& name)
		: Layer(name)
	{
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
		if (Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE))
		{
			m_camera->updateCameraDirection(Input::getMouseDx(), Input::getMouseDy());
			event.isHandled = true;
		}
	}

	void updateCameraMovement()
	{
		const float movementSpeed = 50.0f * m_deltaTime;
		if (Input::isKeyDown(GLFW_KEY_W) == true)
		{
			m_camera->updateCameraPosition(CameraMovement::FORWARD);
		}
		if (Input::isKeyDown(GLFW_KEY_S) == true)
		{
			m_camera->updateCameraPosition(CameraMovement::BACKWARD);
		}
		if (Input::isKeyDown(GLFW_KEY_A) == true)
		{
			m_camera->updateCameraPosition(CameraMovement::LEFT);
		}
		if (Input::isKeyDown(GLFW_KEY_D) == true)
		{
			m_camera->updateCameraPosition(CameraMovement::RIGHT);
		}
	}


	void onEvent(Event& event) override
	{
		ViewPortsController* viewPortsHolder = ViewPortsHolderContext::s_viewPortsController.get();

		if (event.getType() == EventType::MouseButtonPress ||
			event.getType() == EventType::MouseScroll ||
			event.getType() == EventType::MouseButtonRelease ||
			event.getType() == EventType::MouseMove)
		{
			//mozno miesto toho aby sme mali toto tu
			//tak to mozeme priamo zavolat u ViewPortsController
			//ktory nasledne aplikuje tool
			ViewPortsHolderContext::s_viewPortsController->m_currentTool;
			ITool* currentTool = viewPortsHolder->m_currentTool;
			OpParams* currentToolParams = viewPortsHolder->m_currentToolParams;

			if(currentTool != nullptr)
			{
				if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
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

				if (Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
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

				if (Input::isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT))
				{
					if (currentToolParams != nullptr)
					{
						currentTool->getInteractionHandler()->onEnd(*currentToolParams);
					}
					else
					{
						currentTool->getInteractionHandler()->onEnd();
					}
				}

				event.isHandled = true;
			}
			updateCameraDirection(event);
		}
	}

};