#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"

class SelectMeshCommand : public Observable, public Command<SelectionToolParams> {
public:
	void execute(const SelectionToolParams& iParams) override;

	void undo() override;
};
