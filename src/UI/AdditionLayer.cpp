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
    //CHATOVICA - TODO


    auto *addPlaneCommand = CommandRegistry::instance().getCommand(ADD_PLANE_COMMAND);
    if (!addPlaneCommand)
        return;

    ImVec2 windowSize = ImGui::GetWindowSize();

    // --- Push smaller font scale ---
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // default font, can replace later with a smaller font
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));

    // --- Push orange theme for buttons and sliders ---
    ImVec4 orangeColor = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, orangeColor);
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(1.0f, 0.7f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, orangeColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.7f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.1f, 1.0f));

    // --- Calculate content height with smaller widgets ---
    float sliderHeight = 20;
    float inputHeight = 20;
    float buttonHeight = 20;
    float spacing = 5;

    float contentHeight = 
        sliderHeight + spacing + // Subdivision slider
        sliderHeight + spacing + // Size slider
        inputHeight + spacing +  // InputText
        buttonHeight + spacing*2; // Button + extra spacing

    float startY = (windowSize.y - contentHeight) * 0.5f;
    ImGui::SetCursorPosY(startY);

    float widgetWidth = 150; // smaller than before
    float buttonWidth = 100;

    // --- Subdivision ---
    ImGui::SetCursorPosX((windowSize.x - widgetWidth) * 0.5f);
    ImGui::Text("Subdivision");
    ImGui::SetCursorPosX((windowSize.x - widgetWidth) * 0.5f);
    ImGui::PushItemWidth(widgetWidth);
    ImGui::SliderInt("##SubdivisionSlider", &m_subdivision, 1, 300, "%d");
    ImGui::PopItemWidth();

    ImGui::Spacing();

    // --- Size ---
    ImGui::SetCursorPosX((windowSize.x - widgetWidth) * 0.5f);
    ImGui::Text("Size");
    ImGui::SetCursorPosX((windowSize.x - widgetWidth) * 0.5f);
    ImGui::PushItemWidth(widgetWidth);
    ImGui::SliderFloat("##SizeSlider", &m_size, 1.0f, 1000.0f, "%.0f");
    ImGui::PopItemWidth();

    ImGui::Spacing();

    // --- InputText ---
    static char buffer[9] = "";
    ImGui::SetCursorPosX((windowSize.x - widgetWidth) * 0.5f);
    ImGui::Text("Enter Size");
    ImGui::SetCursorPosX((windowSize.x - widgetWidth) * 0.5f);
    ImGui::PushItemWidth(widgetWidth);
    ImGui::InputText("##EnterSizeInput", buffer, IM_ARRAYSIZE(buffer));
    ImGui::PopItemWidth();

    ImGui::Spacing();

    // --- Button ---
    ImGui::SetCursorPosX((windowSize.x - buttonWidth) * 0.5f);
    if (ImGui::Button("AddToScene", ImVec2(buttonWidth, buttonHeight)))
    {
        PlaneParams addPlaneCommandParams;
        addPlaneCommandParams.m_subdivisionLevel = m_subdivision;
        addPlaneCommandParams.m_size = m_size;
        addPlaneCommand->execute(addPlaneCommandParams);

        VisibilityHandler::hide(ADDITION_LAYER);
    }

    // --- Pop styles and font ---
    ImGui::PopStyleColor(5); // orange colors
    ImGui::PopStyleVar(2);   // padding & spacing
    ImGui::PopFont();
}

void AdditionLayer::addCube()
{
    //TODO
}

void AdditionLayer::addSurface()
{
    //TODO
}

void AdditionLayer::setWindowSizeAndPosition()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    float viewportWidth = viewport->WorkSize.x;
    float viewportHeight = viewport->WorkSize.y;

    float width  = viewportWidth * 0.2f;
    float height = viewportHeight * 0.2f;

    float posX = viewport->WorkPos.x + (viewportWidth - width) * 0.45f;
    float posY = viewport->WorkPos.y + (viewportHeight - height) * 0.55f;

    ImGui::SetNextWindowSize({width, height}, ImGuiCond_Always);
    ImGui::SetNextWindowPos({posX, posY}, ImGuiCond_Always);

    WindowStyle::checkResolutionRange(viewportHeight, viewportWidth, ADDITION_LAYER);
}

void AdditionLayer::onImGuiRender()
{
    if (!VisibilityHandler::isVisible(ADDITION_LAYER))
    {
        return;
    }

    setWindowSizeAndPosition();

    int windowAppliedColorStyles{}, windowAppliedVarStyles{};

    static const ImGuiWindowFlags flags = WindowStyle::windowWithTitleBar();
    WindowStyle::setDefaultTitleBar(windowAppliedColorStyles);    
    WindowStyle::setup("Enter object parameters", flags, windowAppliedColorStyles, windowAppliedVarStyles);

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