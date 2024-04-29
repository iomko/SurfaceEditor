#pragma once
#include <map>
#include <string>
#include "Patterns/Command.h"

class ShortcutRegistry {
public:
    static void registerShortcut(const std::initializer_list<int>& shortcut, Command* command) {
        std::string shortcutString;
        for (int key : shortcut) {
            shortcutString += std::to_string(key);
        }
        if (m_shortcuts.find(shortcutString) == m_shortcuts.end()) {
            m_shortcuts[shortcutString] = command;
        }
    }

    static void changeShortcut(const std::initializer_list<int>& oldShortcut, const std::initializer_list<int>& newShortcut) {
        std::string oldShortcutString, newShortcutString;

        for (int key : oldShortcut) {
            oldShortcutString += std::to_string(key);
        }

        for (int key : newShortcut) {
            newShortcutString += std::to_string(key);
        }

        auto it = m_shortcuts.find(oldShortcutString);
        if (it != m_shortcuts.end()) {
            Command* command = it->second;

            m_shortcuts.erase(it);
            m_shortcuts[newShortcutString] = command;
        }
    }

    static void unregisterShortcut(const std::initializer_list<int>& shortcut) {
        std::string shortcutString;
        for (int key : shortcut) {
            shortcutString += std::to_string(key);
        }
        m_shortcuts.erase(shortcutString);
    }

    static Command* getCommand(const std::initializer_list<int>& shortcut) {
        std::string shortcutString;
        for (int key : shortcut) {
            shortcutString += std::to_string(key);
        }

        auto it = m_shortcuts.find(shortcutString);
        if (it != m_shortcuts.end())
        {
            return it->second;
        }
        return nullptr;
    }

private:
    static inline std::map<std::string, Command*> m_shortcuts;
};