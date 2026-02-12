#pragma once

#include "Callbacks/Callback.h"
#include "Params/OperationParams.h"
#include "Patterns/Observer.h"
#include "CallbackIDs.h"

class SolidifyMeshesCallBack : public Callback<SOLIDIFY_MESHES_CALLBACK>, public Observer {

public:

	void execute() override;
}; 
