#pragma once
#include "../Params/OperationParams.h"
#include "CallbackIDs.h"
#include "../Patterns/Observer.h"
#include "Callback.h"

class MoveMeshCallBack : public Callback<MOVE_MESH_CALLBACK, MoveMeshParams>, public Observer
{
public:
    void execute(const MoveMeshParams& iParams) override;
};
