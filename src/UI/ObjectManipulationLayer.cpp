#include "ObjectManipulationLayer.h"
#include "../ViewPortsController.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"
#include "LayerRegistry.h"

namespace
{
    static constexpr const char* cursor = "../images/gui/cursor.png";
    static constexpr const char* translate = "../images/gui/translate.png";
    static constexpr const char* rotate = "../images/gui/rotate.png";
    static constexpr const char* scale = "../images/gui/scale.png";
    static constexpr const char* plus = "../images/gui/plus.png";
}

static AutoRegisterLayerArgs<ObjectManipulationLayer, std::string> reg;

ObjectManipulationLayer::ObjectManipulationLayer(const std::string& name)
    : LayerWithID(name), m_imagesLoaded{}, m_iconSize{} {}

void ObjectManipulationLayer::loadPanelImages()
{
    VisibilityHandler::show(OBJECT_MANIPULATION_LAYER);

    ImVec2 buttonSize{m_iconSize, m_iconSize};

    m_buttons.emplace_back(std::make_unique<ImageButton>(cursor, buttonSize, [](ImageButton* button) {
        //TODO
    }, []() {}));
    m_buttons.emplace_back(std::make_unique<ImageButton>(translate, buttonSize, [](ImageButton* button) {
        //TODO
    }, []() {}));
    m_buttons.emplace_back(std::make_unique<ImageButton>(rotate, buttonSize, [](ImageButton* button) {
        //TODO
    }, []() {}));
    m_buttons.emplace_back(std::make_unique<ImageButton>(scale, buttonSize, [](ImageButton* button) {
        //TODO
    }, []() {}));
    m_buttons.emplace_back(std::make_unique<ImageButton>(plus, buttonSize, [](ImageButton* button) {
        VisibilityHandler::show(OBJECTS);
        button->isSelected() = false;
    }, []() {
        VisibilityHandler::hide(OBJECTS);
    }));

    m_imagesLoaded = true;
}

const ImGuiWindowFlags& ObjectManipulationLayer::setWindowPosition()
{
    static constexpr const ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoCollapse;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    float viewportWidth  = viewport->WorkSize.x;
    float viewportHeight = viewport->WorkSize.y;

    m_iconSize = viewportWidth * 0.015f;

    float framePadding  = ImGui::GetStyle().FramePadding.x;
    float windowPadding = ImGui::GetStyle().WindowPadding.x;

    float windowWidth  = m_iconSize + 2.0f * framePadding + 2.0f * windowPadding;
    float windowHeight = m_buttons.size() * (m_iconSize + 2.0f * framePadding) + 2.0f * windowPadding;

    float leftInset = viewportWidth * 0.02f;

    float posX = viewport->WorkPos.x + leftInset;
    float posY = viewport->WorkPos.y + (viewportHeight - windowHeight) * 0.5f;

    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Always);

    return flags;
}

void ObjectManipulationLayer::onImGuiRender()
{
    const ImGuiWindowFlags& flags = setWindowPosition();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.17f, 0.85f));
    ImGui::Begin(this->getName().c_str(), nullptr, flags);

    if (!m_imagesLoaded)
    {
        loadPanelImages();
    }

    for (auto& button : m_buttons)
    {
        if (button->isSelected())
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.65f, 0.10f, 0.55f));
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        }
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.55f, 0.0f, 0.35f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.55f, 0.0f, 0.55f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 16.0f);

        const int appliedStylesCount = 3;

        if (ImGui::ImageButton(button->name().c_str(), button->textureID(), ImVec2(m_iconSize, m_iconSize)))
        {
            if (button->isSelected())
            {
                button->isSelected() = false;
            }
            else
            {
                for (auto& unselectedButton : m_buttons)
                {
                    unselectedButton->isSelected() = false;
                    unselectedButton->end();
                }

                button->isSelected() = true;
                button->execute();
            }
        }

        ImGui::PopStyleColor(appliedStylesCount);
        ImGui::PopStyleVar();
    }

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}
