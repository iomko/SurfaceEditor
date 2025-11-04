
#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class CreatePrintCommand : public Observable, public Command<PrintMeshSettingsParams>
{
public:
	virtual void execute(const PrintMeshSettingsParams& params) override;

	void undo() override;
};


