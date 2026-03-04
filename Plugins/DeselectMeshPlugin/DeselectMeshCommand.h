#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class DeselectMeshCommand : public Observable, public Command<DESELECT_MESH_COMMAND> {
public:
	virtual void execute() override;

	void undo() override;
};
