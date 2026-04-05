#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Tools/SelectionHandler.h"
#include "../../src/Tools/Tool.h"

class FaceSelectionTool : public Tool<CommandConcept, SelectionHandler<CommandConcept>>
{
public:
	FaceSelectionTool(CommandConcept* command);
};
