#pragma once

#include "Callback.h"
#include "Params/OperationParams.h"
#include "../Patterns/Observer.h"

#include "CallbackIDs.h"

class CreatePrintStructureCallBack : public Callback<CREATE_PRINT_STRUCTURE_CALLBACK, PrintMeshSettingsParams>, public Observer {

public:
	void execute(const PrintMeshSettingsParams& iParams) override;
};
