#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class SelectMeshCommand : public Observable, public Command<SELECT_MESH_COMMAND> {
public:
	void execute() override;

	void undo() override;
};
