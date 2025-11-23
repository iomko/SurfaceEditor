#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "CommandIDs.h"

class DeselectMeshCommand : public Observable, public Command<DESELECT_MESH_COMMAND> {
public:
	virtual void execute() override;

	void undo() override;
};
