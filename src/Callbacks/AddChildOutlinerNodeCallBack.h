#pragma once
#include "../Callbacks/Callback.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Params/OperationParams.h"
#include "CallbackIDs.h"

#include "../UI/OutlinerLayer.h"
template<typename T>
class AddChildOutlinerNodeCallBack : public Callback<-1, AddChildOutlinerNodeCallBackParams<T>>, public Observable
{
public:
	void execute(const AddChildOutlinerNodeCallBackParams<T>& iParams) override
    {
        auto* layerState = dynamic_cast<OutlinerLayerState*>(iParams.state);
        OutlinerNodeConcept* parent = iParams.parent;
        int id = iParams.id;
        std::string name = iParams.name;
        T* data = iParams.data;
        if(layerState == nullptr || data == nullptr)
            return;

        if(parent == nullptr)
            parent = layerState->m_currentSelectedNode;
        if(parent == nullptr)
            return;

        auto* node = new OutlinerNode<T>(id, name, data);
        parent->m_childs.emplace_back(node);
    }
};