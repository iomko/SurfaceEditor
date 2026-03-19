#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Params/OperationParams.h"

class DeleteMeshCallBack : public Callback<MeshParams>, public Observer
{
public:
    DeleteMeshCallBack();

	void execute(const MeshParams& iParams) override;

private:
};
