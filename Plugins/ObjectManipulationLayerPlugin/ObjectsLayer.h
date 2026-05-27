#pragma once
#include <imgui.h>
#include "ObjectManipulationLayer.h"
#include "../AdditionLayerPlugin/AdditionLayer.h"
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/VisibilityHandler.h"
#include "../src/UI/IWindow.h"

class ObjectsLayer : public Layer, public Observable, public ui::IWindow
{
public:
    ObjectsLayer(const std::string& name);

    void onImGuiRender() override;

    void setOnFinishCallback(std::function<void()> onFinishCallback);

    void setPosCallback(std::function<ImVec2()> getPosCallback);

protected:
    void initWindowConfig() override;

    void initComponents() override;

private:
    void updatePosition();

    void invokeAdditionLayer();

private:
    std::function<void()>   m_onFinish;
    std::function<ImVec2()> m_getPos;
};