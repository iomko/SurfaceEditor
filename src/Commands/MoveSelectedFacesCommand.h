#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class MoveSelectedFacesCommand : public Command<MOVE_SELECTED_FACE_COMMAND, MoveSelectedFacesParams>, public Observable
{
public:
	virtual void execute(const MoveSelectedFacesParams& iParams) override;

	void undo() override;
};
