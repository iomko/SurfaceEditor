#pragma once
#include <imgui.h>
#include <stdint.h>
#include <Components/Window.h>
#include <Utils/VisibilityHandler.h>
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/LayerRegistry.h"
#include "../include/enums/AdditionType.h"

class ObjectsLayer : public Layer, public Observable, public ui::components::Window
{
public:
    ObjectsLayer(const std::string& name);

    void onImGuiRender() override;

protected:
    void initWindowConfig() override;

    void initComponents() override;

    std::unique_ptr<ui::placements::Placement> createPlacement() override;

private:
    void invokeAdditionLayer(AdditionType additionType);
};