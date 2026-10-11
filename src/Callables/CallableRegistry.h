#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include "Callable.h"

class CallableRegistry {
public:
    using Creator = std::function<std::unique_ptr<CallableConcept>()>;

    static CallableRegistry& instance() {
        static CallableRegistry registry;
        return registry;
    }

    template<typename CallableT>
    void registerCallable(std::string id) {        
        m_creators[id] = []() -> std::unique_ptr<CallableConcept> {
            return std::make_unique<CallableT>();
        };
    }

    CallableConcept* getCallable(std::string id) {
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
    std::unordered_map<std::string, Creator> m_creators;
    std::unordered_map<std::string, std::unique_ptr<CallableConcept>> m_instances;
};

template<typename CallableT>
struct AutoRegisterCallable {
    AutoRegisterCallable(std::string id) {
        printf("Registering callable with id: %s\n", id.c_str());
        CallableRegistry::instance().registerCallable<CallableT>(id);
    }
};
