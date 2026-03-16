#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "CommandIDs.h"

class DeselectMeshCommand : public Observable, public Command<DESELECT_MESH_COMMAND, SelectionToolParams> {
public:
	virtual void execute(const SelectionToolParams& iParams) override;

	void undo() override;
};
