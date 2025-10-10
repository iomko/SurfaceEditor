#pragma once
#include <string_view>
#include <unordered_map>
#include "Patterns/Command.h"
#include <typeinfo>
#include <typeindex>

class CommandRegistry {
public:
    template<typename CommandT, typename... Args>
    void registerCommand(Args&&... args) {
        m_commands[typeid(CommandT)] = new CommandT(std::forward<Args>(args)...);
    }

    template<typename CommandT>
    CommandT* getCommand() {
        auto it = m_commands.find(typeid(CommandT));
        if (it != m_commands.end()) {
            return static_cast<CommandT*>(it->second);
        }
        return nullptr;
    }

    void deleteRegistry() {
        for (auto& pair : m_commands) {
            delete pair.second;
        }
    }

private:
    std::unordered_map<std::type_index, CommandConcept*> m_commands;
};
