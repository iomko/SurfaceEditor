#pragma once
#include <imgui.h>
#include <stdint.h>
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/VisibilityHandler.h"
#include "../src/UI/IWindow.h"
#include "../include/enums/AdditionType.h"

class ObjectsLayer : public Layer, public Observable, public ui::IWindow
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