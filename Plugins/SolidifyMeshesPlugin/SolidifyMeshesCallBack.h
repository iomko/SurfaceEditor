#pragma once

#include "../../src/Callbacks/Callback.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Callbacks/CallbackIDs.h"

class SolidifyMeshesCallBack : public Callback<SOLIDIFY_MESHES_CALLBACK>, public Observer {

public:

	void execute() override;
}; 
