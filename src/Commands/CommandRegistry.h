#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include "Patterns/Command.h"

class CommandRegistry {
public:
    using Creator = std::function<std::unique_ptr<CommandConcept>()>;

    static CommandRegistry& instance() {
        static CommandRegistry registry;
        return registry;
    }

    template<typename CommandT>
    void registerCommand(const std::string& name) {
        m_creators[name] = []() -> std::unique_ptr<CommandConcept> {
            return std::make_unique<CommandT>();
        };
    }

    CommandConcept* getCommand(const std::string& name) {
        auto it = m_instances.find(name);
        if (it != m_instances.end()) {
            return it->second.get();
        }

        auto itCreator = m_creators.find(name);
        if (itCreator != m_creators.end()) {
            auto instance = itCreator->second();
            auto* rawPtr = instance.get();
            m_instances[name] = std::move(instance);
            return rawPtr;
        }

        return nullptr; // not registered
    }

    void clear() {
        m_instances.clear();
    }

private:
    std::unordered_map<std::string, Creator> m_creators;
    std::unordered_map<std::string, std::unique_ptr<CommandConcept>> m_instances;
};

template<typename CommandT>
struct AutoRegister {
    AutoRegister(const std::string& name) {
        CommandRegistry::instance().registerCommand<CommandT>(name);
    }
};
