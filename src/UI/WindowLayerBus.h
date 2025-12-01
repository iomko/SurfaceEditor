#pragma once
#include <functional>
#include <map>
#include <typeindex>


class LayerState {
};

class WindowLayerBus {
public:
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
