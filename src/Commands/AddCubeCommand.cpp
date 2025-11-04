#include "AddCubeCommand.h"
#include "CommandRegistry.h"
void AddCubeCommand::execute(const CubeParams& params)
{
	notifyObservers(params);
}

void AddCubeCommand::undo()
{
		
}
static AutoRegister<AddCubeCommand> regAddCube("AddCube");