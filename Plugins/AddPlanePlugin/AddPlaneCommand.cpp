#include "AddPlaneCommand.h"
#include "../../src/Commands/CommandRegistry.h"

#include "../../src/Core/Profiler.h"
void AddPlaneCommand::execute(const PlaneParams& params)
{
	notifyObservers(params);
}

void AddPlaneCommand::undo()
{
		
}
static AutoRegister<AddPlaneCommand> regAddPlane("ADD_PLANE_COMMAND");