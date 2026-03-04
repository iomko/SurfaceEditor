
#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class CreatePrintCommand : public Observable, public Command<CREATE_PRINT_COMMAND, PrintMeshSettingsParams>
{
public:
	virtual void execute(const PrintMeshSettingsParams& params) override;

	void undo() override;
};


