#pragma once
#include <iostream>

#include "Renderer.h"
#include "Window.h"
#include "Event.h"
#include "KeyEvent.h"
#include "LayerStack.h"
#include "ImGuiLayer.h"

class Application
{
public:
	Application(int width, int height, const std::string& title)
		: window(width, height, title)
	{
		instance = this;
		window.setEventFunc(std::bind(&Application::onEvent, this, std::placeholders::_1));
		window.initialize();
		window.setCallBackFunctions();
		imGuiLayer = new ImGuiLayer();
		imGuiLayer->onAttach();
		layerStack.addLayer(imGuiLayer);
	}

	static Application& getInstance()
	{
		return *instance;
	}

	Window& getWindow()
	{
		return window;
	}

	void close()
	{
		window.terminate();
	}

	void onAddLayer(Layer* layer)
	{
		layer->onAttach();
		layerStack.addLayer(layer);
	}

	void onAddOverlay(Layer* overlay)
	{
		overlay->onAttach();
		layerStack.addOverlay(overlay);
	}

	void onRemoveLayer(Layer* layer)
	{
		layer->onDetach();
		layerStack.removeLayer(layer);
	}

	void onRemoveOverlay(Layer* overlay)
	{
		overlay->onDetach();
		layerStack.removeOverlay(overlay);
	}

	void onEvent(Event& event)
	{
		//iterate through layerStack in order to propagate events to layerStack, but in reverse
		for(auto it = layerStack.end(); it != layerStack.begin();)
		{
			(*--it)->onEvent(event);
			if (event.isHandled)
			{
				break;
			}
		}

		/*
		std::cout << event.getName() << std::endl;
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressEvent>([](Event& eventType){std::cout << "OUYEE" << std::endl;});
		*/
	}

	void test()
	{
		std::cout << "test" << std::endl;
	}

	void run()
	{
		imGuiLayer->begin();

		for (auto layer : layerStack)
		{
			layer->onImGuiRender();
			layer->onUpdate();
		}

		imGuiLayer->end();

		//glfwSetInputMode(window.windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

private:
	inline static Application* instance = nullptr;
	Window window;
private:
	LayerStack layerStack;
	ImGuiLayer* imGuiLayer = nullptr;
};
