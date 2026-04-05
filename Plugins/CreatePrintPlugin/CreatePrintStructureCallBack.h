#pragma once

#include "../../src/Callbacks/Callback.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Observer.h"

#include "../../src/Callbacks/CallbackIDs.h"

class CreatePrintStructureCallBack : public Callback<PrintMeshSettingsParams>, public Observer {

public:
	void execute(const PrintMeshSettingsParams& iParams) override;
};
