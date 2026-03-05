#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "../Params/OperationParams.h"
#include "CommandIDs.h"

class SplitEdgeCommand : public Command<SPLIT_EDGE_COMMAND, SplitEdgeParams>, public Observable
{
public:
    virtual void execute(const SplitEdgeParams& params) override;

    void undo() override;
};