#pragma once

#include "../../src/Callbacks/Callback.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Observer.h"

class SolidifyMeshesCallBack : public Callback<>, public Observer {

public:

	void execute() override;
}; 
