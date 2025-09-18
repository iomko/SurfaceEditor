
#pragma once
#include "Patterns/Command.h"
#include "Patterns/Observer.h"
#include "Params/OperationParams.h"


class CreatePrintCommand : public Observable, public Command<PrintMeshSettingsParams>
{
public:
	virtual void execute(const PrintMeshSettingsParams& params) override
	{
		notifyObservers(params);
	}

	void undo() override
	{
		
	}

	static constexpr std::string_view getCommandName() noexcept { return "CreatePrintCommand"; }
};


