#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Params/OperationParams.h"

class SplitEdgeCommand : public Command<SplitEdgeParams>, public Observable
{
public:
    virtual void execute(const SplitEdgeParams& params) override;

    void undo() override;
};
