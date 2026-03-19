#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"

class DeleteMeshCommand : public Command<MeshParams>, public Observable
{
public:
	virtual void execute(const MeshParams& params) override;

	void undo() override;
};
