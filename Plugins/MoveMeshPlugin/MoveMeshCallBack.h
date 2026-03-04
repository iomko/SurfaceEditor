#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Callbacks/Callback.h"

class MoveMeshCallBack : public Callback<MOVE_MESH_CALLBACK, MoveMeshParams>, public Observer
{
public:
    void execute(const MoveMeshParams& iParams) override;
};
