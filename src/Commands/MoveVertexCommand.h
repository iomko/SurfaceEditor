#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class MoveVertexCommand : public Command<MOVE_VERTEX_COMMAND ,VertexParams>, public Observable
{
public:
	virtual void execute(const VertexParams& params) override;

	void undo() override;
};
