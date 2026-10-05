#include "AdditionLayer.h"
#include "imgui.h"
#include "../../src/Commands/CommandRegistry.h"

static AutoRegisterLayerArgs<AdditionLayer, std::string> reg("ADDITION_LAYER");

AdditionLayer::AdditionLayer(const std::string& name)
    : Layer(name) {}

// void AdditionLayer::onEvent(Event &event)
// {
//     if (event.getType() == EventType::MouseButtonPress)
//     {
//         if (m_isMouseInsideWindow)
//         {
//             event.isHandled = true;
//         }
//     }
// }

void AdditionLayer::addPlane()
{
    auto* addPlaneCommand = CommandRegistry::instance().getCommand("ADD_PLANE_COMMAND");

    if (addPlaneCommand)
    {
        defaultSettingsWindow(addPlaneCommand, [this]() -> PlaneParams {
            PlaneParams addPlaneCommandParams;
            addPlaneCommandParams.m_subdivisionLevel = m_subdivision;
            addPlaneCommandParams.m_size = m_size;
            addPlaneCommandParams.m_position = glm::vec3(m_xPos, m_zPos, m_yPos);
            
            return addPlaneCommandParams;
        });
    }
}

void AdditionLayer::addCube()
{
    auto* addCubeCommand = CommandRegistry::instance().getCommand("ADD_CUBE_COMMAND");

    if (addCubeCommand)
    {
        defaultSettingsWindow(addCubeCommand, [this]() -> CubeParams {
            CubeParams addCubeParams;
            addCubeParams.m_size = m_size;
            addCubeParams.m_subdivisionLevel = m_subdivision;
            addCubeParams.m_position = glm::vec3(m_xPos, m_zPos, m_yPos);
            
            return addCubeParams;
        });
    }
}

void AdditionLayer::addSurface()
{
    auto* fetchCommand = CommandRegistry::instance().getCommand("FETCH_SURFACE_COMMAND");

    if (!fetchCommand)
    {
        return;
    }

    float windowWidth   = ImGui::GetWindowWidth();
    float windowHeight  = ImGui::GetWindowHeight();
    float panelWidth    = windowWidth * 0.85f;
    float inputBoxWidth = panelWidth * 0.4f;
    float panelHeight   = windowHeight * 0.7f;
    float buttonWidth   = panelWidth * 0.4f;
    float buttonHeight  = panelHeight * 0.2f;
    float spacing       = ImGui::GetStyle().ItemSpacing.x;
    float pairWidth     = inputBoxWidth * 2 + spacing;
    float paddingX      = (panelWidth - pairWidth) * 0.42f;

    int buttonColorsApplied{}, buttonVarsApplied{};

    ImGui::SetCursorPosX((windowWidth - panelWidth) * 0.5f);
    ImGui::SetCursorPosY((windowHeight - panelHeight) * 0.7f);
    ImGui::BeginGroup();

    FontStyle::headliner();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + panelWidth * 0.07f);
    const char* leftCornerText = windowWidth > 300.0f ? "Lower-left corner" : "LL corner";
    ImGui::TextUnformatted(leftCornerText);
    ImGui::SameLine(panelWidth * 0.5f);
    const char* rightCornerText = windowWidth > 300.0f ? "Upper-right corner" : "UR corner";
    ImGui::TextUnformatted(rightCornerText);
    FontStyle::end();

    ImGui::SetNextItemWidth(inputBoxWidth);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, inputBoxBackground);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + paddingX);
    ImGui::Text("Lon LL");
    ImGui::SameLine(0, panelWidth * 0.31f);
    ImGui::Text("Lon UR");

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + paddingX);
    ImGui::SetNextItemWidth(inputBoxWidth);
    ImGui::InputFloat("##Lon LL", &m_lowerLeftLon);
    ImGui::SameLine(0, spacing);
    ImGui::SetNextItemWidth(inputBoxWidth);
    ImGui::InputFloat("##Lon UR", &m_upperRightLon);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + paddingX);
    ImGui::Text("Lat LL");
    ImGui::SameLine(0, panelWidth * 0.31f);
    ImGui::Text("Lat UR");

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + paddingX);
    ImGui::SetNextItemWidth(inputBoxWidth);
    ImGui::InputFloat("##Lat LL", &m_lowerLeftLat);
    ImGui::SameLine(0, spacing);
    ImGui::SetNextItemWidth(inputBoxWidth);
    ImGui::InputFloat("##Lat UR", &m_upperRightLat);

    ImGui::SetCursorPosX(windowWidth * 0.42f);
    ImGui::Text("API key");
    ImGui::SetCursorPosX(windowWidth * 0.33f);
    ImGui::SetNextItemWidth(inputBoxWidth);
    ImGui::InputText("##API Key", m_apiKeyBuffer, IM_ARRAYSIZE(m_apiKeyBuffer));
    ImGui::PopStyleColor();

    ImGui::Spacing();
    WindowStyle::drawHorizontalSeparator(panelWidth);
    ImGui::Spacing();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (panelHeight * 0.05f));
    ImGui::SetCursorPosX(windowWidth * 0.42f);
    ButtonStyle::simplePopUpWindowStyle(buttonHeight, buttonColorsApplied, buttonVarsApplied);
    if (ImGui::Button("Add"))
    {
        OpenTopoParams params;
        params.m_lowerLeftLon = m_lowerLeftLon;
        params.m_lowerLeftLat = m_lowerLeftLat;
        params.m_upperRightLon = m_upperRightLon;
        params.m_upperRightLat = m_upperRightLat;
        params.m_apiKey = std::string(m_apiKeyBuffer);

        fetchCommand->execute(params);

        VisibilityHandler::hide("ADDITION_LAYER");
    }
    ButtonStyle::closeStyling(buttonColorsApplied, buttonVarsApplied);

    ImGui::EndGroup();
}

void AdditionLayer::defaultSettingsWindow(CommandConcept* command, std::function<CommandParams()> paramsCallback)
{
    float windowWidth       = ImGui::GetWindowWidth();
    float windowHeight      = ImGui::GetWindowHeight();
    float panelWidth        = windowWidth * 0.7f;
    float panelHeight       = windowHeight * 0.81f;
    float inputWidth        = panelWidth * 0.31f;
    float sliderWidth       = panelWidth * 0.65;
    float buttonHeight      = panelHeight * 0.15f;
    float buttonWidth       = panelWidth * 0.4f;

    auto inputBoxBackground = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);

    int sliderColorsApplied{};
    int checkboxColorsApplied{}, checkboxVarsApplied{};
    int buttonColorsApplied{}, buttonVarsApplied{};

    ImGui::SetCursorPosX((windowWidth - panelWidth) * 0.5f);
    ImGui::SetCursorPosY((windowHeight - panelHeight) * 0.7f);
    ImGui::BeginGroup();

    // Subdivision//
    int lastSubdivision = m_subdivision;

    FontStyle::headliner();
    ImGui::Text("Subdivision");
    FontStyle::end();

    ImGui::SetNextItemWidth(sliderWidth);
    ImGui::Text("Automatic:");

    ImGui::SameLine();
    CheckBoxStyle::basic(checkboxColorsApplied, checkboxVarsApplied, 1.0f);
    ImGui::Checkbox("##Automatic", &m_automaticSubdivision);
    CheckBoxStyle::end(checkboxColorsApplied, checkboxVarsApplied);

    if (!m_automaticSubdivision)
    {
        ImGui::SetNextItemWidth(inputWidth);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, inputBoxBackground);
        ImGui::InputInt("##SubdivisionInput", &m_subdivision, 0.0f, 0.0f);
        ImGui::PopStyleColor();

        ImGui::SameLine();
        SliderStyle::basic(sliderColorsApplied, sliderWidth);
        ImGui::SliderInt("##Subdivision", &m_subdivision, 1, 300);
        SliderStyle::end(sliderColorsApplied);
    }
    else
    {
        m_subdivision = 1;
        ImGui::Dummy(ImVec2(0.0f, ImGui::GetFrameHeight()));
    }

    if (m_subdivision < 1.0f || m_subdivision > 300)
    {
        m_subdivision = lastSubdivision;
    }
    //----------//

    ImGui::Spacing();
    WindowStyle::drawHorizontalSeparator(panelWidth);
    ImGui::Spacing();

    //Size//
    int lastSize = m_size;

    FontStyle::headliner();
    ImGui::Text("Size");
    FontStyle::end();

    ImGui::SetNextItemWidth(inputWidth);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, inputBoxBackground);
    ImGui::InputFloat("##SizeInput", &m_size, 0.0f, 0.0f, "%.3f");
    ImGui::PopStyleColor();

    ImGui::SameLine();
    SliderStyle::basic(sliderColorsApplied, sliderWidth);
    ImGui::SliderFloat("##Size", &m_size, 1.0f, 1000.0f, "%.0f");
    SliderStyle::end(sliderColorsApplied);

    if (m_size < 1.0f || m_size > 1000.0f)
    {
        m_size = lastSize;
    }
    //---//

    ImGui::Spacing();
    WindowStyle::drawHorizontalSeparator(panelWidth);
    ImGui::Spacing();

    // Position//
    FontStyle::headliner();
    ImGui::Text("Position");
    FontStyle::end();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + panelWidth * 0.01f);
    ImGui::Text("X");
    ImGui::SameLine(panelWidth * 0.34f);
    ImGui::Text("Y");
    ImGui::SameLine(panelWidth * 0.67f);
    ImGui::Text("Z");

    ImGui::SetNextItemWidth(inputWidth);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, inputBoxBackground);
    ImGui::InputFloat("##X", &m_xPos, 0.0f, 0.0, "%.3f");
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, inputBoxBackground);
    ImGui::InputFloat("##Y", &m_yPos, 0.0f, 0.0f, "%.3f");
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, inputBoxBackground);
    ImGui::InputFloat("##Z", &m_zPos, 0.0f, 0.0f, "%.3f");
    ImGui::PopStyleColor();

    ImGui::Spacing();
    WindowStyle::drawHorizontalSeparator(panelWidth);
    ImGui::Spacing();
    //-------//

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (panelHeight * 0.05f));
    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
    ButtonStyle::simplePopUpWindowStyle(buttonHeight, buttonColorsApplied, buttonVarsApplied);
    const char* buttonName = windowWidth > 320.0f ? "Add To Scene" : "Add";
    if (ImGui::Button(buttonName, ImVec2(buttonWidth, 0)))
    {
        CommandParams params = paramsCallback();

        std::visit(
            [&](auto&& p) {
                command->execute(p);
            },
            params
        );

        VisibilityHandler::hide("ADDITION_LAYER");
    }
    ButtonStyle::closeStyling(buttonColorsApplied, buttonVarsApplied);

    ImGui::EndGroup();
}

void AdditionLayer::setWindowSizeAndPosition()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    float viewportWidth  = viewport->WorkSize.x;
    float viewportHeight = viewport->WorkSize.y;

    static constexpr const float fullWidth = 1920.0f;
    float width  = viewportWidth * 0.2f;
    float height = fullWidth * 0.16f;

    float posX = viewport->WorkPos.x + (viewportWidth - width) * 0.5f;
    float posY = viewport->WorkPos.y + (viewportHeight - height) * 0.5f;

    ImGui::SetNextWindowSize({width, height}, ImGuiCond_Always);
    ImGui::SetNextWindowPos({posX, posY}, ImGuiCond_Always);

    float minHeight = 0.8f;
    float minWidth = 0.4f;
    WindowStyle::checkResolutionRange("ADDITION_LAYER", viewportHeight, viewportWidth, minHeight, minWidth);
}

void AdditionLayer::onImGuiRender()
{
    setWindowSizeAndPosition();

    if (!VisibilityHandler::isVisible("ADDITION_LAYER"))
    {
        return;
    }

    bool isWindowTransparent{ true };
    int windowAppliedColorStyles{}, windowAppliedVarStyles{};

    static const ImGuiWindowFlags flags = WindowStyle::windowWithTitleBar();
    
    FontStyle::headliner();
    WindowStyle::setDefaultTitleBar(windowAppliedColorStyles);    
    WindowStyle::setup("Enter object parameters", flags, windowAppliedColorStyles, windowAppliedVarStyles, isWindowTransparent);
    FontStyle::end();
    FontStyle::regular();

    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 mousePos = ImGui::GetMousePos();

    m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                             mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

    switch (s_additionType)
    {
        case AdditionType::PLANE:
            addPlane();
            break;
        case AdditionType::CUBE:
            addCube();
            break;
        case AdditionType::SURFACE:
            addSurface();
            break;
        case AdditionType::NONE:
        default:
            break;
    }

    WindowStyle::end(windowAppliedColorStyles, windowAppliedVarStyles);
    FontStyle::end();
}