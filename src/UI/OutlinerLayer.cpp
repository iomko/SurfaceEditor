#include "OutlinerLayer.h"

static AutoRegisterLayerArgs<OutlinerLayer,std::string, std::reference_wrapper<WindowLayerBus>> reg;

OutlinerLayer::OutlinerLayer(const std::string &name, WindowLayerBus &windowLayerBus)
    : LayerWithID(name), m_windowLayerBus(windowLayerBus) 
    {
    }

void OutlinerLayer::onEvent(Event &event) 
{
    if (event.getType() == EventType::MouseButtonPress)
    {
        if (m_state.m_isMouseInsideWindow)
        {
            event.isHandled = true;
        }
    }
}

void OutlinerLayer::processTree(const std::vector<OutlinerNodeConcept *> &nodes, const ImGuiTreeNodeFlags &imguiFlags, OutlinerLayerState &outlinerLayerState)
{
    for (OutlinerNodeConcept *node : nodes)
    {

        bool expanded = ImGui::TreeNodeEx((void *)(intptr_t)node->m_id, imguiFlags, "%s", node->m_name.c_str());
        if (ImGui::IsItemClicked())
        {
            outlinerLayerState.m_currentSelectedNode = node;

            m_windowLayerBus.emit<OutlinerLayerState>(m_state);
            std::cout << "Selected currentTreeNode" << std::endl;
        }

        if (expanded)
        {
            processTree(node->m_childs, imguiFlags, outlinerLayerState);
            ImGui::TreePop();
        }
    }
}

void OutlinerLayer::onImGuiRender() 
{
    ImGui::Begin(this->getName().c_str());

    // Get window position and size
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 mousePos = ImGui::GetMousePos();

    m_state.m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                                     mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

    ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (!m_state.m_nodes.empty())
    {
        processTree(m_state.m_nodes, base_flags, m_state);
    }

    ImGui::End();
}
void OutlinerLayer::editParams(OpParams& iParams)
{
    AddOutlinerNodeCallBackParams& params = dynamic_cast<AddOutlinerNodeCallBackParams&>(iParams);
    params.state = &m_state;
}