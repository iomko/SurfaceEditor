#pragma once

#include "../../src/Callbacks/Callback.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/UI/WindowLayerBus.h"

class RemoveOutlinerNodeCallBack : public Callback<RemoveOutlinerNodeParams>, public Observable
{
public:
    void execute(const RemoveOutlinerNodeParams& params) override;

private:
    static bool containsNode(const std::vector<OutlinerNodeConcept*>& nodes, OutlinerNodeConcept* target);
    static bool isRemovedNode(
        const std::vector<OutlinerNodeConcept*>& nodes,
        void* data,
        OutlinerNodeConcept* target);
    static bool removeNode(std::vector<OutlinerNodeConcept*>& nodes, void* data);
};
