#pragma once
#include <string>
#include <functional>

class Button
{
public:
    Button(
        const std::string name,
        std::function<void(Button*)> callback,
        std::function<void()> endCallback
    ) : m_name{ name }, m_callback{ callback }, m_endCallback{ endCallback } {}

    virtual ~Button() = default;

    inline const std::string& name() const { return m_name; }

    inline void execute() { m_callback(this); }

    inline void end() { m_endCallback(); };

private:
    std::string m_name;
    std::function<void(Button*)> m_callback;
    std::function<void()> m_endCallback;
};