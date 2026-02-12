#pragma once
#include "Callback.h"
#include "../Patterns/Observer.h"
#include "../Params/OperationParams.h"
#include "CallbackIDs.h"

class DeleteMeshCallBack : public Callback<DELETE_MESH_CALLBACK, MeshParams>, public Observer
{
public:
    DeleteMeshCallBack();

	void execute(const MeshParams& iParams) override;

private:
};
