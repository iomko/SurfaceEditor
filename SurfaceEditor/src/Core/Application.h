#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include <iostream>
import LayerSystem.Layer.ImGuiLayer;
import LayerSystem.Layer;


class Application
{
public:
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

	static Window& getWindow()
	{
		return *m_window;
	}

	void close()
	{
		m_window->terminate();
		m_window = nullptr;
	}

	void onAddLayer(Layer* layer)
	{
		layer->onAttach();
		m_layerStack.addLayer(layer);
	}

	void onAddOverlay(Layer* overlay)
	{
		overlay->onAttach();
		m_layerStack.addOverlay(overlay);
	}

	void onRemoveLayer(Layer* layer)
	{
		layer->onDetach();
		m_layerStack.removeLayer(layer);
	}

	void onRemoveOverlay(Layer* overlay)
	{
		overlay->onDetach();
		m_layerStack.removeOverlay(overlay);
	}

	void onEvent(Event& event)
	{
		for(auto layerIt = m_layerStack.end(); layerIt != m_layerStack.begin();)
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
	inline static Window* m_window = nullptr;
	LayerStack m_layerStack;
	ImGuiLayer* m_imGuiLayer = nullptr;
};

#endif