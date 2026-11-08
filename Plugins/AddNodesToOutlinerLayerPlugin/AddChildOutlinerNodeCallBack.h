#pragma once
#include "../../src/Patterns/Observer.h"
#include "../../src/UI/WindowLayerBus.h"

template<typename T>
class AddChildOutlinerNodeCallBack : public Callback<AddChildOutlinerNodeCallBackParams<T>>, public Observable
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

        for (OutlinerNodeConcept* existing : parent->m_childs)
        {
            auto* typedNode = dynamic_cast<OutlinerNode<T>*>(existing);
            if (typedNode != nullptr && typedNode->m_data == data)
            {
                return;
            }
        }

        auto* node = new OutlinerNode<T>(id, name, data);
        parent->m_childs.emplace_back(node);
    }
};