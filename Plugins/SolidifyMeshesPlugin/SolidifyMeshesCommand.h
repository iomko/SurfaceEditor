#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class SolidifyMeshesCommand : public Command<SOLIDIFY_MESHES_COMMAND>, public Observable
{
public:
	virtual void execute() override;

	void undo() override;
};
