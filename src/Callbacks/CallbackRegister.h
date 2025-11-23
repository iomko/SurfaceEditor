#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include "Callback.h"

class CallbackRegistry {
public:
    using Creator = std::function<std::unique_ptr<CallbackConcept>()>;

    static CallbackRegistry& instance() {
        static CallbackRegistry registry;
        return registry;
    }

    template<typename CallbackT>
    void registerCallback() {        
        m_creators[CallbackT::ID] = []() -> std::unique_ptr<CallbackConcept> {
            return std::make_unique<CallbackT>();
        };
    }

    void registerCallbackFactory(int id, Creator factory) {
        m_creators[id] = std::move(factory);
    }
    CallbackConcept* getCallback(int id) {
        auto it = m_instances.find(id);
        if (it != m_instances.end()) {
            return it->second.get();
        }

        auto itCreator = m_creators.find(id);
        if (itCreator != m_creators.end()) {
            auto instance = itCreator->second();
            auto* rawPtr = instance.get();
            m_instances[id] = std::move(instance);
            return rawPtr;
        }

        return nullptr; // not registered
    }

    void clear() {
        m_instances.clear();
    }

private:
    std::unordered_map<int, Creator> m_creators;
    std::unordered_map<int, std::unique_ptr<CallbackConcept>> m_instances;
};

template<typename CallbackT>
struct AutoRegisterCallback {
    AutoRegisterCallback() {
        CallbackRegistry::instance().registerCallback<CallbackT>();
    }
};
