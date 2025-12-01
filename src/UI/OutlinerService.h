#pragma once
#include <string>
#include "OutlinerModel.h"
//UROB AKO CALLABLE
namespace Outliner {
    inline OutlinerLayerState* s_state = nullptr;

    inline void registerState(OutlinerLayerState* s) { s_state = s; }
    inline OutlinerLayerState* getState() { return s_state; }

    template<typename T>
    OutlinerNodeConcept* addNode(int id, const std::string& name, const T& data) {
        if (!s_state) 
        {
            printf(" OutlinerNodeConcept* addNode NO STATE\n");
            return nullptr;
        }
        return createNode<T>(*s_state, id, name, data);
    }

    template<typename T>
    OutlinerNodeConcept* addChildNode(OutlinerNodeConcept* parent, int id, const std::string& name, const T& data) {
        return createChildNode<T>(parent, id, name, data);
    }
}