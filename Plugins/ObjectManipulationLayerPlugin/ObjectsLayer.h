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

private:
    void invokeAdditionLayer(AdditionType additionType);

    void updatePos(ImVec2& actualPosition);
};