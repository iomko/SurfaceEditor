#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/UI/WindowLayerBus.h"

template<typename T>
class AddNewOutlinerNodeCallBack : public Callback<-1, AddNewOutlinerNodeCallBackParams<T>>, public Observable
{
public:
	void execute(const AddNewOutlinerNodeCallBackParams<T>& iParams) override
    {
        OutlinerLayerState* layerState = dynamic_cast<OutlinerLayerState*>(iParams.state);
        int id = iParams.id;
        std::string name = iParams.name;
        T* data = iParams.data;
        if(layerState == nullptr || data == nullptr)
            return;

        auto* node = new OutlinerNode<T>(id, name, data);
        layerState->m_nodes.emplace_back(node);
    }
};