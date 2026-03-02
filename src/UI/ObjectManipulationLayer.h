#pragma once
#include <imgui.h>
#include <vector>
#include <memory>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "LayerRegistry.h"
#include "LayerIDs.h"
#include "Components/IWindow.h"
#include "Components/ImageButton.h"
#include "Components/WindowStyle.h"
#include "Components/ButtonStyle.h"
#include "VisibilityHandler.h"


class ObjectManipulationLayer : public LayerWithID<OBJECT_MANIPULATION_LAYER>, public Observable, public IWindow
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