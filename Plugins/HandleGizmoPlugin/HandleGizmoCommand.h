#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"

class HandleGizmoCommand : public Command<GizmoParams>, public Observable
{
public:
	virtual void execute(const GizmoParams& iParams) override;

	void undo() override;
};
