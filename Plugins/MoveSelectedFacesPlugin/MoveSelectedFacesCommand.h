#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"

class MoveSelectedFacesCommand : public Command<MoveSelectedFacesParams>, public Observable
{
public:
	virtual void execute(const MoveSelectedFacesParams& iParams) override;

	void undo() override;
};
