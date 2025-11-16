#pragma once
#include "Patterns/Command.h"
#include "Patterns/Observer.h"

class SolidifyMeshesCommand : public Command<SOLIDIFY_MESHES_COMMAND>, public Observable
{
public:
	virtual void execute() override;

	void undo() override;
};
