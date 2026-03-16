#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "CommandIDs.h"

class SelectMeshCommand : public Observable, public Command<SELECT_MESH_COMMAND, SelectionToolParams> {
public:
	void execute(const SelectionToolParams& iParams) override;

	void undo() override;
};
