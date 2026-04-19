#include "ObjectsLayer.h"
#include "../src/Commands/CommandRegistry.h"
#include "../src/Commands/CommandIDs.h"

static AutoRegisterLayerArgs<ObjectsLayer, std::string> reg("OBJECTS_LAYER");

ObjectsLayer::ObjectsLayer(const std::string& name)
    : Layer(name)
{
    // auto& layerRegistry = LayerRegistry::instance();
    // auto layer = layerRegistry.getLayer("OBJECT_MANIPULATION_LAYER", "ObjectManipulationLayer");
    // auto objectManipulationLayer = static_cast<ObjectManipulationLayer*>(layer);

    // m_windowPos = objectManipulationLayer->rightBottomCorner();
}

void ObjectsLayer::onImGuiRender()
{
    //TODO
}