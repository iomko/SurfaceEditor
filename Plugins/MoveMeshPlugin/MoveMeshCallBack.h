#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Callbacks/Callback.h"

class MoveMeshCallBack : public Callback<MoveMeshParams>, public Observer
{
public:
    void execute(const MoveMeshParams& iParams) override;
};
