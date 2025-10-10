#pragma once

class AddPlaneCommand : public Command<PlaneParams>, public Observable
{
public:
	virtual void execute(const PlaneParams& params) override
	{
		notifyObservers(params);
	}

	void undo() override
	{
		
	}
};
