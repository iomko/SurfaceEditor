#include "PluginAPI.h"
#include "../Callbacks/CallbackRegister.h"
#include "../Commands/CommandRegistry.h"
#include "../Tools/ToolRegistry.h"
#include "../Patterns/Observer.h"
#include "../UI/WindowLayerBus.h"
#include "../../src/UI/LayerRegistry.h"
#include "Application.h"

void setupLayerPlugin(std::string layer_id, const std::string name, bool useWindowLateyBus)
{
	WindowLayerBus &bus = WindowLayerBus::instance();
	Application &app = Application::getInstance();
	Layer *layer;
	if (useWindowLateyBus == false)
	{
		layer = LayerRegistry::instance().getLayer(layer_id, name);
	}
	else
	{
		layer = LayerRegistry::instance().getLayer(layer_id, name, std::ref(bus));
	}
	if (layer)
		app.getLayerStack().addLayer(layer);
}

void setupPlugin(std::string command_id, std::string callback_id, std::string tool_id)
{
	auto *callback = CallbackRegistry::instance().getCallback(callback_id);
	if (callback == nullptr)
	{
		printf("not callback with id %s\n", callback_id.c_str());
		return;
	}
	auto *command = CommandRegistry::instance().getCommand(command_id); // zjednotit + osobitny .h ako ciselnik a robit cez id
	auto *observableCommand = dynamic_cast<Observable *>(command);
	auto *observerCallback = dynamic_cast<Observer *>(callback);
	if (observableCommand && observerCallback)
	{
		observableCommand->addObserver(observerCallback);
		observerCallback->observe(observableCommand, callback);
		if (tool_id != "")
		{
			ToolRegistry::instance().initializeTool(tool_id, command);
		}
	}
}