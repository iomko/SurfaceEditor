#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class SelectMeshCommand : public Observable, public Command<SELECT_MESH_COMMAND> {
public:
	void execute() override;

	void undo() override;
};
