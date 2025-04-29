#pragma once
#include <vector>
#include "Patterns/Observer.h"
#include "Patterns/Command.h"
#include "Core/Layer.h"
#include "Scene/Scene.h"
#include "Scene/ViewPortLayerRenderSettings.h"
#include "Scene/ViewPortLayerScreenSettings.h"
#include "../ObjectSelectionHolder.h"

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
	ICommand* m_currentCommand = nullptr;
	Params* m_currentCommandParams = nullptr;
	std::set<Mesh*> m_selectedMeshes;
	std::map<Mesh*, std::set<HalfEdgeDS::Face*>> m_selectedFaces;
	std::map<Mesh*, std::set<HalfEdgeDS::Vertex*>> m_selectedVertices;
	std::vector<ICommand*> m_commandsQueue;
	Scene* m_scene = nullptr;

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
				std::cout << "ViewPortsOnButtonPressLEFTEvent" << std::endl;
				if (ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand != nullptr)
				{
					if (ViewPortsHolderContext::m_viewPortsHolder->m_currentCommandParams == nullptr)
					{
						ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand->execute();
						event.isHandled = true;
					}
					else
					{
						ViewPortsHolderContext::m_viewPortsHolder->m_currentCommand->execute(
							*ViewPortsHolderContext::m_viewPortsHolder->m_currentCommandParams);
						event.isHandled = true;
					}
				}
			}
		}

		if (event.getType() == EventType::MouseButtonPress || event.getType() == EventType::MouseMove)
		{
			updateCameraDirection(event);
		}
	}

};