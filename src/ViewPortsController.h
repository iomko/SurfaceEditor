#pragma once

#include <algorithm>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <Components/Window.h>
#include "Editing/Selection/SelectionController.h"
#include "Patterns/Observer.h"
#include "Scene/ViewPortLayerRenderSettings.h"
#include "Tools/Tool.h"
#include "Core/Layer.h"
#include "Scene/Object.h"

class Event;

class ViewPortLayer;

class ViewPortsController : public Observer
{
public:
    ViewPortsController() = default;

    void addLayer(ViewPortLayer* viewPortLayer)
    {
        m_viewPortLayers.push_back(viewPortLayer);
    }

    std::vector<ViewPortLayer*> m_viewPortLayers;
    ViewPortLayer* m_activeViewPortLayer = nullptr;

    std::vector<CommandConcept*> m_commandsQueue;
    Scene* m_scene = nullptr;

    ITool* m_currentTool = nullptr;
    OpParams* m_currentToolParams = nullptr;

    Object* addObject(std::unique_ptr<Object> object)
    {
        if (!object)
        {
            throw std::invalid_argument("Cannot add a null object.");
        }

        Object* rawObject = object.get();
        m_objects.push_back(std::move(object));
        return rawObject;
    }

    Object* createObject(const std::string& name)
    {
        return addObject(std::make_unique<Object>(name));
    }

    bool removeObject(Object* object)
    {
        const auto it = std::find_if(
            m_objects.begin(),
            m_objects.end(),
            [object](const std::unique_ptr<Object>& candidate)
            {
                return candidate.get() == object;
            });
        if (it == m_objects.end() || !(*it)->getMeshes().empty())
        {
            return false;
        }

        m_objects.erase(it);
        return true;
    }

private:
    std::vector<std::unique_ptr<Object>> m_objects;
};

class ViewPortsUILayerController
{
public:
    void registerUiWindow(ui::components::Window* uiWindow)
    {
        m_uiWindow.push_back(uiWindow);
    }

    bool clickedOnUiWindow(const glm::vec2& clickPos)
    {
        for (auto& window : m_uiWindow)
        {
            if (window->isVisible() && window->clickedOnWindow(clickPos))
            {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<ui::components::Window*> m_uiWindow;
};

class ViewPortsHolderContext
{
public:
    static inline Window* s_window = nullptr;
    static inline std::unique_ptr<ViewPortsUILayerController> s_uiLayerController = std::make_unique<ViewPortsUILayerController>();
    static inline std::unique_ptr<ViewPortsController> s_viewPortsController = std::make_unique<ViewPortsController>();
    static inline std::unique_ptr<SelectionController> s_selectionController = std::make_unique<SelectionController>();
    static inline std::unique_ptr<Camera> s_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 17.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    static inline void shutdown()
    {
        s_uiLayerController.reset();
        s_viewPortsController.reset();
        s_selectionController.reset();
        s_camera.reset();
    }
};


class ViewPortLayer : public Layer, public Observable
{
private:
    float m_deltaTime = 0.0f;

public:
    ViewPortLayerRenderSettings m_shaderSettings;
    Camera* m_camera = nullptr;

    ViewPortLayer(const std::string& name);

    void onUpdate() override;
    void updateCameraDirection(Event& event);
    void updateCameraMovement();
    void onEvent(Event& event) override;
};
