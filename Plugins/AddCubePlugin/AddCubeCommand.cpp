#include "AddCubeCommand.h"
#include "../../src/Commands/CommandRegistry.h"
void AddCubeCommand::execute(const CubeParams& params)
{
	notifyObservers(params);
}

void AddCubeCommand::undo()
{
		
}
static void test() {
	printf("Test\n");
}
static AutoRegister<AddCubeCommand> regAddCube;