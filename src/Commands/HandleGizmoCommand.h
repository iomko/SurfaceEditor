#pragma once
#include "../Params/OperationParams.h"
#include "../Patterns/Command.h"
#include "../Patterns/Observable.h"
#include "CommandIDs.h"

class HandleGizmoCommand : public Command<HANDLE_GIZMO_COMMAND, GizmoParams>, public Observable
{
public:
	virtual void execute(const GizmoParams& iParams) override;

	void undo() override;
};
