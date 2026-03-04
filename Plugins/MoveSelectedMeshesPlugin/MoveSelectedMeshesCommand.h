#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class MoveSelectedMeshesCommand : public Command<MOVE_SELECTED_MESHES_COMMAND, MoveSelectedMeshesParams>, public Observable
{
public:
	virtual void execute(const MoveSelectedMeshesParams& iParams);

	void undo() override;
};
