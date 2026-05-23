#pragma once
#include <imgui.h>
#include <glm/glm.hpp>
#include "Styling/Window.h"
#include "Components/Common.h"

class IWindow
{
public:
    IWindow(ui::styling::WindowConfig config);

    virtual ~IWindow() = default;

    bool clickedOnWindow(const glm::vec2& clickPos);

protected:
    virtual ui::styling::WindowConfig initWindowConfig() = 0;

    virtual void initComponents() = 0;

    void renderComponents();

    template<typename T, typename... Args>
    T* emplaceComponent(Args&&... args)
    {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* rawPtr = component.get();
        m_components.push_back(std::move(component));
        return rawPtr;
    }

protected:
    ui::styling::WindowConfig                                m_windowConfig;
    std::vector<std::unique_ptr<ui::components::IComponent>> m_components;
};