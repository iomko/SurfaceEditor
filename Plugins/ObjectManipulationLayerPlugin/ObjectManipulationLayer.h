#pragma once
#include <imgui.h>
#include <vector>
#include <memory>
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/Components/IWindow.h"
#include "../src/UI/Components/ImageButton.h"
#include "../src/UI/Components/WindowStyle.h"
#include "../src/UI/Components/ButtonStyle.h"
#include "../src/UI/VisibilityHandler.h"
#include "../src/UI/OverlappingWindow.h"


class ObjectManipulationLayer : public Layer, public Observable, public OverlappingWindow, public IWindow
{
public:
    ObjectManipulationLayer(const std::string& name);

    inline ImVec2* rightBottomCorner() { return &m_rightBottomCorner; }

    void onImGuiRender() override;

private:
    void loadPanelImages();

    void setWindowSizeAndPosition() override;

private:
    std::vector<std::unique_ptr<ImageButton>> m_buttons;
    ImVec2 m_rightBottomCorner;
    bool m_imagesLoaded;
    float m_iconSize;
};