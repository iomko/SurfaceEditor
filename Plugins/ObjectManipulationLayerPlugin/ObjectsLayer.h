#pragma once
#include <imgui.h>
#include "ObjectManipulationLayer.h"
#include "../AdditionLayerPlugin/AdditionLayer.h"
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/VisibilityHandler.h"

class ObjectsLayer : public Layer, public Observable
{
public:
    ObjectsLayer(const std::string& name);

    void onImGuiRender() override;
};