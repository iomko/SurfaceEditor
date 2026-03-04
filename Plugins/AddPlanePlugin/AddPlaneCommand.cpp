#include "AddPlaneCommand.h"
#include "../../src/Commands/CommandRegistry.h"
void AddPlaneCommand::execute(const PlaneParams& params)
{
	notifyObservers(params);
}

void AddPlaneCommand::undo()
{
		
}
static AutoRegister<AddPlaneCommand> regAddPlane;