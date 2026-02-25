#pragma once
#include <imgui.h>
#include <vector>
#include <memory>
#include "../Patterns/Observer.h"
#include "../Core/Layer.h"
#include "LayerRegistry.h"
#include "LayerIDs.h"
#include "Components/ImageButton.h"
#include "VisibilityHandler.h"


class ObjectManipulationLayer : public LayerWithID<OBJECT_MANIPULATION_LAYER>, public Observable
{
public:
    ObjectManipulationLayer(const std::string& name);

    void onImGuiRender() override;

private:
    void loadPanelImages();

    const ImGuiWindowFlags& setWindowPosition();

private:
    std::vector<std::unique_ptr<ImageButton>> m_buttons;
    bool m_imagesLoaded;
    float m_iconSize;
};