#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class MoveVertexCommand : public Command<MOVE_VERTEX_COMMAND ,VertexParams>, public Observable
{
public:
	virtual void execute(const VertexParams& params) override;

	void undo() override;
};
