#include "ObjectsLayer.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"
#include "LayerRegistry.h"

static AutoRegisterLayerArgs<ObjectsLayer, std::string> reg;

ObjectsLayer::ObjectsLayer(const std::string& name)
    : LayerWithID(name), m_windowSize{}
{
    auto& layerRegistry = LayerRegistry::instance();
    auto layer = layerRegistry.getLayer(OBJECT_MANIPULATION_LAYER, "ObjectManipulationLayer");
    auto objectManipulationLayer = static_cast<ObjectManipulationLayer*>(layer);

    m_windowPos = objectManipulationLayer->rightBottomCorner();
}

void ObjectsLayer::setWindowSizeAndPosition()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    float viewportWidth = viewport->WorkSize.x;
    float viewportHeight = viewport->WorkSize.y;

    float width  = viewportWidth * 0.05f;
    float height = viewportHeight * 0.09f;

    m_windowSize = { width, height };

    ImGui::SetNextWindowSize(m_windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(*m_windowPos, ImGuiCond_Always);

    WindowStyle::checkResolutionRange(viewportHeight, viewportWidth, OBJECTS);
}

void ObjectsLayer::onImGuiRender()
{
    setWindowSizeAndPosition();

    if (!VisibilityHandler::isVisible(OBJECTS))
    {
        return;
    }


    int windowStyleColorApplied{}, windowStyleVarApplied{};
    int styleColorApplied{}, styleVarApplied{};
    int buttonCount{3};

    static const ImGuiWindowFlags flags = WindowStyle::defaultWindow();
    WindowStyle::setup(this->getName().c_str(), flags, windowStyleColorApplied, windowStyleVarApplied);

    auto layout = ButtonStyle::calculateVerticalButtonLayout(buttonCount);
    ButtonStyle::applyResponsiveFontScale(layout.height);
    ButtonStyle::simplePopUpWindowStyle(layout.height, styleColorApplied, styleVarApplied);

    if (ImGui::Button("Plane", ImVec2(layout.width, layout.height)))
    {
        AdditionLayer::setAdditionType(AdditionType::PLANE);
        VisibilityHandler::show(ADDITION_LAYER);
        VisibilityHandler::hide(OBJECTS);
    }
    if (ImGui::Button("Cube", ImVec2(layout.width, layout.height)))
    {
        AdditionLayer::setAdditionType(AdditionType::CUBE);
        VisibilityHandler::show(ADDITION_LAYER);
        VisibilityHandler::hide(OBJECTS);
    }
    if (ImGui::Button("Surface", ImVec2(layout.width, layout.height)))
    {
        AdditionLayer::setAdditionType(AdditionType::SURFACE);
        VisibilityHandler::show(ADDITION_LAYER);
        VisibilityHandler::hide(OBJECTS);
    }

    ButtonStyle::closeStyling(styleColorApplied, styleVarApplied);

    WindowStyle::end(windowStyleColorApplied, windowStyleVarApplied);
}