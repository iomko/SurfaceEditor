#include "ObjectsLayer.h"
#include "../src/Commands/CommandRegistry.h"
#include "../src/Commands/CommandIDs.h"
#include "../src/ViewPortsController.h"

static AutoRegisterLayerArgs<ObjectsLayer, std::string> reg("OBJECTS_LAYER");

ObjectsLayer::ObjectsLayer(const std::string& name)
    : Layer(name),
      OverlappingWindow(initWindowConfig())
{
    ViewPortsHolderContext::s_uiLayerController->registerUiWindow(this);
}

void ObjectsLayer::updatePosition(float x, float y)
{
    m_posX = x;
    m_posY = y;
}

ui::styling::WindowConfig ObjectsLayer::initWindowConfig()
{
    //TODO

    return {};
}

void ObjectsLayer::onImGuiRender()
{
    if (!VisibilityHandler::isVisible("OBJECTS_LAYER"))
    {
        return;
    }

    //TODO
}