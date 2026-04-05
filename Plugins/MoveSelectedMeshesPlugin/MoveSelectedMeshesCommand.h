#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"

class MoveSelectedMeshesCommand : public Command<MoveSelectedMeshesParams>, public Observable
{
public:
	virtual void execute(const MoveSelectedMeshesParams& iParams);

	void undo() override;
};
