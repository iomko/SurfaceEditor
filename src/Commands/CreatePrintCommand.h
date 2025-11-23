
#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "CommandIDs.h"

class CreatePrintCommand : public Observable, public Command<CREATE_PRINT_COMMAND, PrintMeshSettingsParams>
{
public:
	virtual void execute(const PrintMeshSettingsParams& params) override;

	void undo() override;
};


