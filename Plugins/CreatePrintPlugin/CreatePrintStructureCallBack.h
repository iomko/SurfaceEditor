#pragma once

#include "../../src/Callbacks/Callback.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Observer.h"

#include "../../src/Callbacks/CallbackIDs.h"

class CreatePrintStructureCallBack : public Callback<CREATE_PRINT_STRUCTURE_CALLBACK, PrintMeshSettingsParams>, public Observer {

public:
	void execute(const PrintMeshSettingsParams& iParams) override;
};
