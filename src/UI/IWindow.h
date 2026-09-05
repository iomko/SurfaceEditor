#pragma once
#include <imgui.h>
#include <glm/glm.hpp>
#include "Styling/Window.h"
#include "Layout.h"

namespace ui
{

    class IWindow
    {
    public:
        IWindow(const std::string& layerName);

        virtual ~IWindow() = default;

        bool clickedOnWindow(const glm::vec2& clickPos);

        bool isVisible();

    protected:
        virtual void initWindowConfig() = 0;

        virtual void initComponents() = 0;

        void render();

        Layout* emplaceLayout(const ImVec2& margin = {});

        template<typename T, typename... Args>
        T* emplaceComponent(Layout* layout, Args&&... args)
        {
            layout->asignComponent(std::make_unique<T>(std::forward<Args>(args)...));
            T* component = dynamic_cast<T*>(layout->components().back().get());
            return component;
        }

    protected:
        ui::styling::WindowConfig            m_windowConfig;
        std::vector<std::unique_ptr<Layout>> m_layouts;

    private:
        std::string m_layerName;
    };

} // ui
