#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class DeleteSelectedMeshesCommand : public Command<DELETE_SELECTED_MESHES_COMMAND>, public Observable
{
public:
	virtual void execute() override;

	void undo() override;
};
