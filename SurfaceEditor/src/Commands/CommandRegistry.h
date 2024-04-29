#pragma once

#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include "../Patterns/Command.h"


class CommandRegistry {
public:

    template<typename CommandT, typename... Args>
    static void registerCommand(Args&&... args) {
        constexpr std::string_view commandName = CommandT::getCommandName();
        m_commands[commandName] = new CommandT(std::forward<Args>(args)...);
    }

    template<typename CommandT>
    static CommandT* getCommand() {
        auto it = m_commands.find(CommandT::getCommandName());
        if (it != m_commands.end()) {
            return static_cast<CommandT*>(it->second);
        }
        return nullptr;
    }

    static void deleteRegistry() {
        for (auto& pair : m_commands) {
            delete pair.second;
        }
    }

private:
    static inline std::unordered_map<std::string_view, Command*> m_commands;
};
