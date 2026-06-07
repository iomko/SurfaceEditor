#include "IWindow.h"
#include "../ViewPortsController.h"

namespace ui
{

    IWindow::IWindow(const std::string& layerName)
        : m_layerName(layerName)
    {
        ViewPortsHolderContext::s_uiLayerController->registerUiWindow(this);
    }

    bool IWindow::clickedOnWindow(const glm::vec2& clickPos)
    {
        ImVec2 bottomRight = ImVec2(
            m_windowConfig.pos.rawPos.x + m_windowConfig.size.rawSize.x,
            m_windowConfig.pos.rawPos.y + m_windowConfig.size.rawSize.y
        );
        
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 pos = viewport->Pos;

        ImVec2 actualClickPos(pos.x + clickPos.x, pos.y + clickPos.y); 

        return actualClickPos.x >= m_windowConfig.pos.rawPos.x && actualClickPos.x <= bottomRight.x &&
            actualClickPos.y >= m_windowConfig.pos.rawPos.y && actualClickPos.y <= bottomRight.y;
    }

    void IWindow::render()
    {
        if (!m_windowConfig.pos.relativePosition)
        {
            ui::styling::Window::setPosAndSize(m_windowConfig);
        }
        else
        {
            ui::styling::Window::setRelativePosAndSize(m_windowConfig);
        }

        if (!VisibilityHandler::isVisible(m_layerName))
        {
            return;
        }

        ui::styling::Window::init(m_windowConfig);

        for (auto& layout : m_layouts)
        {
            if (!layout->isVisible())
            {
                continue;
            }

            ui::styling::Window::addToLayout([&]() {
                for (auto& component : layout->components())
                {
                    if (component->isVisible())
                    {
                        component->render();
                    }
                    else
                    {
                        ImGui::Dummy(ImVec2{ 0.0f, ImGui::GetFrameHeight() });
                    }
                }
            }, layout->margin());
        }
        
        ui::styling::Window::destroy(m_windowConfig);
    }

    Layout* IWindow::emplaceLayout(const ImVec2& margin)
    {
        m_layouts.emplace_back(std::make_unique<Layout>(margin));
        Layout* layout = m_layouts.back().get();
        return layout;
    }

} // ui