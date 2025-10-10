#pragma once

class DeleteSelectedMeshesCommand : public Command<>, public Observable
{
public:
	virtual void execute() override
	{
		notifyObservers();
	}

	void undo() override
	{

	}
};
