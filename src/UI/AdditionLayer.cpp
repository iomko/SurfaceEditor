#include "AdditionLayer.h"
#include "imgui.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"

static AutoRegisterLayerArgs<AdditionLayer, std::string> reg;

AdditionLayer::AdditionLayer(const std::string& name)
    : LayerWithID(name) {}

void AdditionLayer::onEvent(Event &event)
{
    if (event.getType() == EventType::MouseButtonPress)
    {
        if (m_isMouseInsideWindow)
        {
            event.isHandled = true;
        }
    }
}

void AdditionLayer::addPlane()
{
    auto* addPlaneCommand = CommandRegistry::instance().getCommand(ADD_PLANE_COMMAND);

    if (addPlaneCommand)
    {
        defaultSettingsWindow(addPlaneCommand, [this]() -> PlaneParams {
            PlaneParams addPlaneCommandParams;
            addPlaneCommandParams.m_subdivisionLevel = m_subdivision;
            addPlaneCommandParams.m_size = m_size;
            
            return addPlaneCommandParams;
        });
    }
}

void AdditionLayer::addCube()
{
    auto* addCubeCommand = CommandRegistry::instance().getCommand(ADD_CUBE_COMMAND);

    if (addCubeCommand)
    {
        defaultSettingsWindow(addCubeCommand, [this]() -> CubeParams {
            CubeParams addCubeParams;
            addCubeParams.m_size = m_size;
            addCubeParams.m_subdivisionLevel = m_subdivision;
            addCubeParams.m_position = glm::vec3(m_xPos, m_yPos, m_zPos);
            
            return addCubeParams;
        });
    }
}

void AdditionLayer::addSurface()
{
    //TODO
}

void AdditionLayer::defaultSettingsWindow(CommandConcept* command, std::function<CommandParams()> paramsCallback)
{
    float windowWidth   = ImGui::GetWindowWidth();
    float windowHeight  = ImGui::GetWindowHeight();
    float panelWidth    = windowWidth * 0.8f;
    float panelHeight   = windowHeight * 0.81f;
    float inputWidth    = panelWidth * 0.3f;
    float sliderWidth   = panelWidth;
    float buttonHeight  = panelHeight * 0.15f;
    float buttonWidth   = panelWidth * 0.4f;

    auto inputBoxBackground = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);

    int sliderColorsApplied{};
    int checkboxColorsApplied{}, checkboxVarsApplied{};
    int buttonColorsApplied{}, buttonVarsApplied{};

    ImGui::SetCursorPosX((windowWidth - panelWidth) * 0.5f);
    ImGui::SetCursorPosY((windowHeight - panelHeight) * 0.7f);
    ImGui::BeginGroup();

    // Subdivision//
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
        SliderStyle::basic(sliderColorsApplied, sliderWidth);
        ImGui::SliderInt("##Subdivision", &m_subdivision, 1, 300);
        SliderStyle::end(sliderColorsApplied);
    }
    else
    {
        m_subdivision = 1;
        ImGui::Dummy(ImVec2(0.0f, ImGui::GetFrameHeight()));
    }
    //----------//

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Size//
    FontStyle::headliner();
    ImGui::Text("Size");
    FontStyle::end();

    ImGui::SetNextItemWidth(inputWidth);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, inputBoxBackground);
    ImGui::InputFloat("##SizeInput", &m_size, 0.0f, 0.0f, "%.5f");
    ImGui::PopStyleColor();

    SliderStyle::basic(sliderColorsApplied, sliderWidth);
    ImGui::SliderFloat("##Size", &m_size, 1.0f, 1000.0f, "%.0f");
    SliderStyle::end(sliderColorsApplied);

    if (m_size < 1.0f)
    {
        m_size = 1.0f;
    }
    //---//

    ImGui::Spacing();
    ImGui::Separator();
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
    ImGui::InputInt("##X", &m_xPos, 0.0f, 0.0f);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, inputBoxBackground);
    ImGui::InputInt("##Y", &m_yPos, 0.0f, 0.0f);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, inputBoxBackground);
    ImGui::InputInt("##Z", &m_zPos, 0.0f, 0.0f);
    ImGui::PopStyleColor();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    //-------//

    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
    ButtonStyle::simplePopUpWindowStyle(buttonHeight, buttonColorsApplied, buttonVarsApplied);
    if (ImGui::Button("Add To Scene", ImVec2(buttonWidth, 0)))
    {
        CommandParams params = paramsCallback();

        std::visit(
            [&](auto&& p) {
                command->execute(p);
            },
            params
        );

        VisibilityHandler::hide(ADDITION_LAYER);
    }
    ButtonStyle::closeStyling(buttonColorsApplied, buttonVarsApplied);

    ImGui::EndGroup();
}

void AdditionLayer::setWindowSizeAndPosition()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    float viewportWidth = viewport->WorkSize.x;
    float viewportHeight = viewport->WorkSize.y;

    float width  = viewportWidth * 0.2f;
    float height = viewportHeight * 0.37f;

    float posX = viewport->WorkPos.x + (viewportWidth - width) * 0.5f;
    float posY = viewport->WorkPos.y + (viewportHeight - height) * 0.5f;

    ImGui::SetNextWindowSize({width, height}, ImGuiCond_Always);
    ImGui::SetNextWindowPos({posX, posY}, ImGuiCond_Always);

    float minHeight = 0.8f;
    float minWidth = 0.4f;
    WindowStyle::checkResolutionRange(ADDITION_LAYER, viewportHeight, viewportWidth, minHeight, minWidth);
}

void AdditionLayer::onImGuiRender()
{
    setWindowSizeAndPosition();

    if (!VisibilityHandler::isVisible(ADDITION_LAYER))
    {
        return;
    }

    int windowAppliedColorStyles{}, windowAppliedVarStyles{};

    static const ImGuiWindowFlags flags = WindowStyle::windowWithTitleBar();
    
    FontStyle::headliner();
    WindowStyle::setDefaultTitleBar(windowAppliedColorStyles);    
    WindowStyle::setup("Enter object parameters", flags, windowAppliedColorStyles, windowAppliedVarStyles);
    FontStyle::end();
    FontStyle::regular();

    //////TOTO SKONTROLOVAT NA CO TO VLASTNE PYTA//////
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 mousePos = ImGui::GetMousePos();

    m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                             mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);
    //////////////////////////////////////////////////////

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

// void AdditionLayer::onImGuiRender()
// {
//     if (!VisibilityHandler::isVisible(ADDITION_LAYER))
//     {
//         return;
//     }

//     ImGui::Begin(this->getName().c_str());

//     ImVec2 windowPos = ImGui::GetWindowPos();
//     ImVec2 windowSize = ImGui::GetWindowSize();
//     ImVec2 mousePos = ImGui::GetMousePos();

//     m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
//                              mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

//     if (ImGui::TreeNode("Add"))
//     {
//         if (ImGui::TreeNode("Mesh"))
//         {

//             auto *addPlaneCommand = CommandRegistry::instance().getCommand(ADD_PLANE_COMMAND);
//             if(addPlaneCommand)
//             {
//                 if (ImGui::TreeNode("TriangulatedPlane"))
//                 {
//                     ImGui::SliderInt("Subdivision", &m_subdivision, 1, 300);
//                     ImGui::SliderFloat("Size", &m_size, 1.0f, 1000.0f, "%.0f");

//                     if (ImGui::Button("AddToScene"))
//                     {
//                         PlaneParams addPlaneCommandParams;
//                         addPlaneCommandParams.m_subdivisionLevel = m_subdivision;
//                         addPlaneCommandParams.m_size = m_size;
//                         addPlaneCommand->execute(addPlaneCommandParams);
//                     }

//                     ImGui::TreePop();
//                 }
//             }
//             auto *addCubeCommand = CommandRegistry::instance().getCommand(ADD_CUBE_COMMAND);
//             if (addCubeCommand)
//             {
//                 if (ImGui::TreeNode("Cube"))
//                 {
//                     ImGui::SliderInt("Subdivision", &m_subdivision, 1, 300);
//                     ImGui::SliderFloat("Size", &m_size, 1.0f, 1000.0f, "%.0f");

//                     ImGui::InputInt3("Position", glm::value_ptr(m_position));
//                     if (ImGui::Button("AddToScene"))
//                     {                        
//                         CubeParams addCubeParams;
//                         addCubeParams.m_size = m_size;
//                         addCubeParams.m_subdivisionLevel = m_subdivision;
//                         addCubeParams.m_position = glm::vec3(m_position);
//                         addCubeCommand->execute(addCubeParams);
//                     }
//                     ImGui::TreePop();
//                 }
//             }
            

//             ImGui::TreePop();
//         }
//         auto *fetchCommand = CommandRegistry::instance().getCommand(FETCH_SURFACE_COMMAND);
//         if(fetchCommand)
//         {
//             if (ImGui::TreeNode("Fetch Surface Data (OpenTopography)"))
//             {
//                 ImGui::Text("Lower-left corner:");
//                 ImGui::InputFloat("Lon LL", &m_lowerLeftLon);
//                 ImGui::InputFloat("Lat LL", &m_lowerLeftLat);

//                 ImGui::Text("Upper-right corner:");
//                 ImGui::InputFloat("Lon UR", &m_upperRightLon);
//                 ImGui::InputFloat("Lat UR", &m_upperRightLat);

//                 ImGui::InputText("API Key", m_apiKeyBuffer, IM_ARRAYSIZE(m_apiKeyBuffer));

//                 if (ImGui::Button("Fetch and Add to Scene"))
//                 {
//                     OpenTopoParams params;
//                     params.m_lowerLeftLon = m_lowerLeftLon;
//                     params.m_lowerLeftLat = m_lowerLeftLat;
//                     params.m_upperRightLon = m_upperRightLon;
//                     params.m_upperRightLat = m_upperRightLat;
//                     params.m_apiKey = std::string(m_apiKeyBuffer);

//                     fetchCommand->execute(params);
//                 }

//                 ImGui::TreePop();
//             }
//         }
        
//         ImGui::TreePop();
//     }

//     ImGui::End();
// }