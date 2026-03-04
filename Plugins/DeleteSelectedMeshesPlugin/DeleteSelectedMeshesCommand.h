#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class DeleteSelectedMeshesCommand : public Command<DELETE_SELECTED_MESHES_COMMAND>, public Observable
{
public:
	virtual void execute() override;

	void undo() override;
};
