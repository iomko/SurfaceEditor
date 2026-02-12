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
    void registerCommand() {        
        m_creators[CommandT::ID] = []() -> std::unique_ptr<CommandConcept> {
            return std::make_unique<CommandT>();
        };
    }

    CommandConcept* getCommand(int id) {
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
    std::unordered_map<int, std::unique_ptr<CommandConcept>> m_instances;
};

template<typename CommandT>
struct AutoRegister {
    AutoRegister() {
        CommandRegistry::instance().registerCommand<CommandT>();
    }
};
