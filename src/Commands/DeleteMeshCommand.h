#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class DeleteMeshCommand : public Command<MeshParams>, public Observable
{
public:
	virtual void execute(const MeshParams& params) override;

	void undo() override;
};
