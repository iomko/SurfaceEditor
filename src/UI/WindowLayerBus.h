#pragma once
#include <functional>
#include <map>
#include <typeindex>
#include "../Core/Layer.h"

struct OutlinerNodeConcept {
    OutlinerNodeConcept(int id, const std::string& name)
        : m_id(id), m_name(name) {}
    virtual ~OutlinerNodeConcept() = default;

    int m_id;
    std::string m_name;
    std::vector<OutlinerNodeConcept*> m_childs;
};

template<typename T>
class OutlinerNode : public OutlinerNodeConcept {
public:
    OutlinerNode(int id, const std::string& name, T* data)
        : OutlinerNodeConcept(id, name), m_data(data) {}
    T* m_data;
};

struct LayerState {
    virtual ~LayerState() = default;
};

struct OutlinerLayerState : LayerState {
    std::vector<OutlinerNodeConcept*> m_nodes;
    OutlinerNodeConcept* m_currentSelectedNode = nullptr;
    bool m_isMouseInsideWindow = false;
};

class WindowLayerBus {
public:
    static WindowLayerBus& instance()
    {
        static WindowLayerBus bus;
        return bus;
    }
    template<typename State>
    static void emit(State& state) {
        for(auto& handler : m_handlers[typeid(State)]) {
            handler(state);
        }
    }

    template<typename State> 
    static void on(std::function<void(State&)> func) {
        m_handlers[std::type_index(typeid(State))].emplace_back(
            [func](LayerState& base) {
                func(static_cast<State&>(base));
            }
        );
    }

    

private:
    inline static std::map<std::type_index, std::vector<std::function<void(LayerState&)>>> m_handlers; 
};
