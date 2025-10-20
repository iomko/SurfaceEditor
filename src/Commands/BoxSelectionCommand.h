#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "../Params/OperationParams.h"

class BoxSelectionCommand : public Observable, public Command<BoxSelectionParams> {
public:
	virtual void execute(const BoxSelectionParams& params) override {
		notifyObservers(params);
	}

	void undo() override {
	}

};
