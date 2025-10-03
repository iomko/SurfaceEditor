#pragma once

#include "Window.h"
#include <iostream>

#include "Layer.h"
#include "../UI/ImGuiLayer.h"


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
		if (!window) {
			throw std::runtime_error("Application window is not initialized!");
		}
		return *window;
	}

	void close()
	{
		if (window) {
			window->terminate();
			delete window;
			window = nullptr;
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
		window = new Window(width, height, title);
		window->setEventFunc(std::bind(&Application::onEvent, this, std::placeholders::_1));
		window->initialize();
		m_imGuiLayer = new ImGuiLayer("ImGuiLayer");
		m_imGuiLayer->onAttach();
		m_layerStack.addOverlay(m_imGuiLayer);
		glfwSetInputMode(window->getWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	~Application()
	{
		close(); // Ensure resources are cleaned up
	}

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

public:
	inline static Window* window = nullptr;
private:
	LayerStack m_layerStack;
	ImGuiLayer* m_imGuiLayer = nullptr;
};
