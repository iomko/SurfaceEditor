#pragma once
#include "../Callbacks/Callback.h"
#include "../Callables/FunctionComposer.h"
#include "../Patterns/Observer.h"
#include "../Commands/AddCubeCommand.h"
#include "../Commands/CommandRegistry.h"

class AddCubeCallback : public ComposedCallback<CubeParams>, public Observer
{
public:
	AddCubeCallback(const FunctionComposer& functionComposer)
		: ComposedCallback(functionComposer)
	{
		
	}
	void registerToCommand(CommandRegistry& commandRegistry)
	{
		commandRegistry.registerCommand<AddCubeCommand>();
		auto* addCubeCommand = commandRegistry.getCommand<AddCubeCommand>();

		addCubeCommand->addObserver(this);
		this->observe(addCubeCommand, this);
	}
};