#pragma once
#include <string>

#include "../Callbacks/CallbackRegister.h"
#include "../Commands/CommandRegistry.h"
#include "../Tools/ToolRegistry.h"
#include "../Patterns/Observer.h"

void setupPlugin(const int command_id, const int callback_id, const int tool_id = -1)
{
	auto *callback = CallbackRegistry::instance().getCallback(callback_id);
	if (callback == nullptr)
	{
		printf("not callback with id %d\n", callback_id);
		return;
	}
	auto *command = CommandRegistry::instance().getCommand(command_id); // zjednotit + osobitny .h ako ciselnik a robit cez id
	auto *observableCommand = dynamic_cast<Observable *>(command);
	auto *observerCallback = dynamic_cast<Observer *>(callback);
	if (observableCommand && observerCallback)
	{
		observableCommand->addObserver(observerCallback);
		observerCallback->observe(observableCommand, callback);
		if (tool_id != -1)
		{
			ToolRegistry::instance().initializeTool(tool_id, command);
		}
	}
}

class IPlugin
{
public:
    virtual ~IPlugin() = default;
    virtual void OnLoad() = 0;
    virtual void OnUnload() = 0;
    virtual std::string GetName() const = 0;
};

using CreatePluginFn = IPlugin* (*)();