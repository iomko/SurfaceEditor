#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class DeselectMeshCommand : public Observable, public Command<DESELECT_MESH_COMMAND> {
public:
	virtual void execute() override;

	void undo() override;
};
