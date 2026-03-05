#pragma once
#include "Patterns/Command.h"
#include "Patterns/Observer.h"
#include "CommandIDs.h"

class SolidifyMeshesCommand : public Command<SOLIDIFY_MESHES_COMMAND>, public Observable
{
public:
	virtual void execute() override;

	void undo() override;
};
