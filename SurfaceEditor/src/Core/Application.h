#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include <iostream>

#include "Layer.h"
#include "../Gui/ImGuiLayer.h"


class Application
{
public:
	static Application& getInstance(int width = 800, int height = 600, const std::string& title = "Application")
	{
		static Application instance(width, height, title);
		return instance;
	}

	static Window& getWindow()
	{
		if (!m_window) {
			throw std::runtime_error("Application window is not initialized!");
		}
		return *m_window;
	}

	void close()
	{
		if (m_window) {
			m_window->terminate();
			delete m_window;
			m_window = nullptr;
		}
	}

	LayerStack& getLayerStack()
	{
		return m_layerStack;
	}

	void onEvent(Event& event)
	{
		for (auto layerIt = m_layerStack.end(); layerIt != m_layerStack.begin();)
		{
			(*--layerIt)->onEvent(event);
			if (event.isHandled)
			{
				break;
			}
		}
	}

	void run()
	{
		m_imGuiLayer->begin();

		for (auto layer : m_layerStack) {
			layer->onImGuiRender();
			layer->onUpdate();
		}
		m_imGuiLayer->end();
	}

private:
	Application(int width, int height, const std::string& title)
	{
		m_window = new Window(width, height, title);
		m_window->setEventFunc(std::bind(&Application::onEvent, this, std::placeholders::_1));
		m_window->initialize();
		m_imGuiLayer = new ImGuiLayer("ImGuiLayer");
		m_imGuiLayer->onAttach();
		m_layerStack.addOverlay(m_imGuiLayer);
		glfwSetInputMode(m_window->getWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	~Application()
	{
		close(); // Ensure resources are cleaned up
	}

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	inline static Window* m_window = nullptr;
	LayerStack m_layerStack;
	ImGuiLayer* m_imGuiLayer = nullptr;
};

#endif
