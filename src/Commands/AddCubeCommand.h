#pragma once

class AddCubeCommand : public Command<CubeParams>, public Observable
{
public:
	virtual void execute(const CubeParams& params) override
	{
		notifyObservers(params);
	}

	void undo() override
	{
		
	}
};
