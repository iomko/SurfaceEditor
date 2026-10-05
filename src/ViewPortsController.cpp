#include "ViewPortsController.h"
#include "Core/Input.h"
#include <iostream>

ViewPortLayer::ViewPortLayer(const std::string& name)
    : Layer(name)
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ViewPortLayer::onUpdate()
{
    updateCameraMovement();
}

void ViewPortLayer::updateCameraDirection(Event& event)
{
    if (Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE))
    {
        m_camera->updateCameraDirection(Input::getMouseDx(), Input::getMouseDy());
        event.isHandled = true;
    }
}

void ViewPortLayer::updateCameraMovement()
{
    const float movementSpeed = 50.0f * m_deltaTime;
    
    if (Input::isKeyDown(GLFW_KEY_W) == true)
    {
        m_camera->updateCameraPosition(CameraMovement::FORWARD);
    }
    if (Input::isKeyDown(GLFW_KEY_S) == true)
    {
        m_camera->updateCameraPosition(CameraMovement::BACKWARD);
    }
    if (Input::isKeyDown(GLFW_KEY_A) == true)
    {
        m_camera->updateCameraPosition(CameraMovement::LEFT);
    }
    if (Input::isKeyDown(GLFW_KEY_D) == true)
    {
        m_camera->updateCameraPosition(CameraMovement::RIGHT);
    }
}

void ViewPortLayer::onEvent(Event& event)
{
    ViewPortsController* viewPortsHolder = ViewPortsHolderContext::s_viewPortsController.get();

    if (event.getType() == EventType::MouseButtonPress ||
        event.getType() == EventType::MouseScroll ||
        event.getType() == EventType::MouseButtonRelease ||
        event.getType() == EventType::MouseMove)
    {
        ITool* currentTool = viewPortsHolder->m_currentTool;
        OpParams* currentToolParams = viewPortsHolder->m_currentToolParams;

        if (currentTool != nullptr)
        {
            if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
            {
                std::cout << "Button_Clicked!" << std::endl;
                if (currentToolParams != nullptr)
                {
                    currentTool->getInteractionHandler()->onBegin(*currentToolParams);
                }
                else
                {
                    currentTool->getInteractionHandler()->onBegin();
                }
            }

            if (Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
            {
                if (currentToolParams != nullptr)
                {
                    currentTool->getInteractionHandler()->onUpdate(*currentToolParams);
                }
                else
                {
                    currentTool->getInteractionHandler()->onUpdate();
                }
            }

            if (Input::isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT))
            {
                if (currentToolParams != nullptr)
                {
                    currentTool->getInteractionHandler()->onEnd(*currentToolParams);
                }
                else
                {
                    currentTool->getInteractionHandler()->onEnd();
                }
            }

            event.isHandled = true;
        }
        
        updateCameraDirection(event);
    }
}
