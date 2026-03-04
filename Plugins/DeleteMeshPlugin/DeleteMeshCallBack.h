#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/CallbackIDs.h"

class DeleteMeshCallBack : public Callback<DELETE_MESH_CALLBACK, MeshParams>, public Observer
{
public:
    DeleteMeshCallBack();

	void execute(const MeshParams& iParams) override;

private:
};
