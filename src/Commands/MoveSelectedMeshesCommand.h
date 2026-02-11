#pragma once
#include "../Params/OperationParams.h"
#include "../Patterns/Command.h"
#include "../Patterns/Observable.h"
#include "CommandIDs.h"

class MoveSelectedMeshesCommand : public Command<OVE_SELECTED_MESHES_COMMAND, MoveSelectedMeshesParams>, public Observable
{
public:
	virtual void execute(const MoveSelectedMeshesParams& iParams);

	void undo() override;
};
