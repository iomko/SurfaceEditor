#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"

class MoveVertexCommand : public Command<VertexParams>, public Observable
{
public:
	virtual void execute(const VertexParams& params) override;

	void undo() override;
};
