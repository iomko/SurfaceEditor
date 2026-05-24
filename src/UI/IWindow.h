#pragma once
#include <imgui.h>
#include <glm/glm.hpp>
#include "Styling/Window.h"
#include "Components/Common.h"

class IWindow
{
public:
    IWindow(const std::string& layerName);

    virtual ~IWindow() = default;

    bool clickedOnWindow(const glm::vec2& clickPos);

protected:
    virtual void initWindowConfig() = 0;

    virtual void initComponents() = 0;

    void render();

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

private:
    std::string m_layerName;
};