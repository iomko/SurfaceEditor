#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class MoveSelectedFacesCommand : public Command<MoveSelectedFacesParams>, public Observable
{
public:
	virtual void execute(const MoveSelectedFacesParams& iParams) override;

	void undo() override;
};
