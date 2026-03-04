#include "ObjectManipulationLayer.h"
#include "../ViewPortsController.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"
#include "LayerRegistry.h"

namespace
{
    //Names
    static constexpr const char* cursor = "cursor";
    static constexpr const char* translate = "translate";
    static constexpr const char* rotate = "rotate";
    static constexpr const char* scale = "scale";
    static constexpr const char* plus = "plus";

    //Paths
    static constexpr const char* cursorPath = "../images/gui/cursor.png";
    static constexpr const char* translatePath = "../images/gui/translate.png";
    static constexpr const char* rotatePath = "../images/gui/rotate.png";
    static constexpr const char* scalePath = "../images/gui/scale.png";
    static constexpr const char* plusPath = "../images/gui/plus.png";
}

static AutoRegisterLayerArgs<ObjectManipulationLayer, std::string> reg;

ObjectManipulationLayer::ObjectManipulationLayer(const std::string& name)
    : LayerWithID(name), m_imagesLoaded{}, m_iconSize{}, m_rightBottomCorner{} {}

void ObjectManipulationLayer::loadPanelImages()
{
    VisibilityHandler::show(OBJECT_MANIPULATION_LAYER);

    m_buttons.emplace_back(std::make_unique<ImageButton>(cursor, cursorPath, [](Button* button) {
        //TODO
    }, []() {}));

    m_buttons.emplace_back(std::make_unique<ImageButton>(translate, translatePath, [](Button* button) {
        //TODO
    }, []() {}));

    m_buttons.emplace_back(std::make_unique<ImageButton>(rotate, rotatePath, [](Button* button) {
        //TODO
    }, []() {}));

    m_buttons.emplace_back(std::make_unique<ImageButton>(scale, scalePath, [](Button* button) {
        //TODO
    }, []() {}));

    m_buttons.emplace_back(std::make_unique<ImageButton>(plus, plusPath, [](Button* button) {
        auto imageButton = dynamic_cast<ImageButton*>(button);
        
        if (imageButton)
        {
            imageButton->isSelected() = false;
        }
        
        VisibilityHandler::show(OBJECTS);
    }, []() {
        VisibilityHandler::hide(OBJECTS);
        VisibilityHandler::hide(ADDITION_LAYER);
    }));

    m_imagesLoaded = true;
}

void ObjectManipulationLayer::setWindowSizeAndPosition()
{
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

    m_rightBottomCorner = { posX + windowWidth - (m_iconSize / 2), posY + windowHeight - (m_iconSize / 2) };

    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Always);

    WindowStyle::checkResolutionRange(viewportHeight, viewportWidth, OBJECT_MANIPULATION_LAYER);
}

void ObjectManipulationLayer::onImGuiRender()
{
    if (!m_imagesLoaded)
    {
        loadPanelImages();
    }

    setWindowSizeAndPosition();

    if (!VisibilityHandler::isVisible(OBJECT_MANIPULATION_LAYER))
    {
        return;
    }
    
    int windowColorStylesApplied{}, windowVarStylesApplied{};

    static const ImGuiWindowFlags flags = WindowStyle::defaultWindow();
    WindowStyle::setup(this->getName().c_str(), flags, windowColorStylesApplied, windowVarStylesApplied);

    for (auto& button : m_buttons)
    {
        int apppliedColorStyles{}, appliedVarStyles{};

        ButtonStyle::setRadioButtonBackground(button.get(), apppliedColorStyles);
        ButtonStyle::controlPanelStyle(apppliedColorStyles, appliedVarStyles);

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

        ButtonStyle::closeStyling(apppliedColorStyles, appliedVarStyles);
    }

    WindowStyle::end(windowColorStylesApplied, windowVarStylesApplied);
}
