#pragma once

class SelectFaceCommand : public Observable, public Command<> {
public:
	virtual void execute() override {
		notifyObservers();
	}

	void undo() override {
	}

};
