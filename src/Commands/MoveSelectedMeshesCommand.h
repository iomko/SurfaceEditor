#pragma once
#include "../Params/OperationParams.h"
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "CommandIDs.h"

class MoveSelectedMeshesCommand : public Command<MOVE_SELECTED_MESHES_COMMAND, MoveSelectedMeshesParams>, public Observable
{
public:
	virtual void execute(const MoveSelectedMeshesParams& iParams);

	void undo() override;
};
