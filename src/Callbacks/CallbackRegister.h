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
    void registerCallback(std::string id) {        
        m_creators[id] = []() -> std::unique_ptr<CallbackConcept> {
            return std::make_unique<CallbackT>();
        };
    }

    void registerCallbackFactory(std::string id, Creator factory) {
        m_creators[id] = std::move(factory);
    }
    CallbackConcept* getCallback(std::string id) {
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

    bool availableID(std::string id)
    {
        return !m_creators.contains(id);
    }

private:
    std::unordered_map<std::string, Creator> m_creators;
    std::unordered_map<std::string, std::unique_ptr<CallbackConcept>> m_instances;
};

template<typename CallbackT>
struct AutoRegisterCallback {
    AutoRegisterCallback(std::string id) {
        CallbackRegistry::instance().registerCallback<CallbackT>(id);
    }
};
