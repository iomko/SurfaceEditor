#pragma once
#include "Button.h"

class RadioButton : public Button
{
public:
    RadioButton(
        const std::string name,
        std::function<void(Button*)> callback,
        std::function<void()> endCallback
    ) : Button(name, callback, endCallback), m_isSelected{} {}

    ~RadioButton() = default;

    inline bool& isSelected() { return m_isSelected; }

private:
    bool m_isSelected;
};