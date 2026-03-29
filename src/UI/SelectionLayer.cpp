#include "SelectionLayer.h"
#include "imgui.h"

static AutoRegisterLayerArgs<SelectionLayer, std::string> reg;

SelectionLayer::SelectionLayer(const std::string &name)
    : LayerWithID(name), m_eventHandled{}
{
    m_selectionRectangle.create();
}

void SelectionLayer::onImGuiRender()
{
    if (!VisibilityHandler::isVisible(SELECTION_LAYER))
    {
        return;
    }

    const bool buttonDown = Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT);

    if (!buttonDown && m_eventHandled)
    {
        return;
    }
    else if (!buttonDown && !m_eventHandled)
    {
        m_selectionRectangle.clear();
        m_selectionRectangle.setActive(false);
        m_eventHandled = true;
    
        SelectionLayerParams toolBarParams;
        toolBarParams.m_rectanglePos  = m_rectanglePos;
        notifyObservers(toolBarParams);
    }
    else if (buttonDown)
    {
        auto mousePos = glm::vec2(Input::getMouseX(), Input::getMouseY());

        if (!m_selectionRectangle.active())
        {
            m_rectanglePos.startPos = mousePos;
            m_rectanglePos.endPos   = mousePos;
            m_selectionRectangle.setActive(true);
            m_eventHandled = false;
        }
        else
        {
            m_rectanglePos.endPos = mousePos;
        }
    }

    drawSelectionRectangle();
}

void SelectionLayer::drawSelectionRectangle()
{
    const int windowWidth  = ViewPortsHolderContext::s_window->getScreenWidth();
    const int windowHeight = ViewPortsHolderContext::s_window->getScreenHeight();

    m_selectionRectangle.update(windowWidth, windowHeight, m_rectanglePos);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto& shader = m_selectionRectangle.shader();
    shader.bind();

    // fill rectangle
    shader.setVec4("u_color", glm::vec4(1.0f, 0.6f, 0.0f, 0.25f));
    Renderer::drawTriangles(m_selectionRectangle.triangleBuffer().data, &shader);

    // draw border
    shader.setVec4("u_color", glm::vec4(1.0f, 0.45f, 0.0f, 1.0f));
    Renderer::drawLineLoop(m_selectionRectangle.linBuffer().data, &shader);

    shader.unbind();

    glEnable(GL_DEPTH_TEST);
}