#include "IWindow.h"
#include "../ViewPortsController.h"

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

    ui::styling::Window::addToLayout([this]() {
        for (auto& component : m_components)
        {
            component->render();
        }
    });
    
    ui::styling::Window::destroy(m_windowConfig);
}
