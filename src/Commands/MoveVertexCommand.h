#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class MoveVertexCommand : public Command<VertexParams>, public Observable
{
public:
	virtual void execute(const VertexParams& params) override;

	void undo() override;
};
