#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class HandleGizmoCommand : public Command<HANDLE_GIZMO_COMMAND, GizmoParams>, public Observable
{
public:
	virtual void execute(const GizmoParams& iParams) override;

	void undo() override;
};
