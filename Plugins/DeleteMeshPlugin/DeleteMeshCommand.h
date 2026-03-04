#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class DeleteMeshCommand : public Command<DELETE_MESH_COMMAND,MeshParams>, public Observable
{
public:
	virtual void execute(const MeshParams& params) override;

	void undo() override;
};
