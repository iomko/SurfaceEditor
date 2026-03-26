#include "SelectionLayer.h"
#include "imgui.h"

static AutoRegisterLayerArgs<SelectionLayer, std::string> reg("SELECTION_LAYER");

SelectionLayer::SelectionLayer(const std::string &name)
    : Layer(name)
{
}

SelectionLayerParams::SelectionMode SelectionLayer::getSelectionMode() const
{
    return m_selectionMode;
}

SelectionLayerParams::Type SelectionLayer::getType() const
{
    return m_type;
}

void SelectionLayer::onImGuiRender()
{
    ImGui::Begin(this->getName().c_str());
    if (ImGui::TreeNode("Selection Mode"))
    {
        int selectionModeInt = static_cast<int>(m_selectionMode);
        if (ImGui::RadioButton("Face Mode", &selectionModeInt, SelectionLayerParams::SelectionMode::Face) ||
            ImGui::RadioButton("Edge Mode", &selectionModeInt, SelectionLayerParams::SelectionMode::Edge) ||
            ImGui::RadioButton("Vertex Mode", &selectionModeInt, SelectionLayerParams::SelectionMode::Vertex) ||
            ImGui::RadioButton("Object Mode", &selectionModeInt, SelectionLayerParams::SelectionMode::Object))
        {
            m_selectionMode = static_cast<SelectionLayerParams::SelectionMode>(selectionModeInt);

            SelectionLayerParams toolBarParams;
            toolBarParams.m_selectionMode = m_selectionMode;
            toolBarParams.m_type = m_type;
            notifyObservers(toolBarParams);
        }
        ImGui::TreePop();
    }

    int typeInt = static_cast<int>(m_type);
    if (ImGui::RadioButton("Selection", &typeInt, SelectionLayerParams::Type::Selection) ||
        ImGui::RadioButton("Deselection", &typeInt, SelectionLayerParams::Type::Deselection))
    {
        m_type = static_cast<SelectionLayerParams::Type>(typeInt);

        SelectionLayerParams toolBarParams;
        toolBarParams.m_selectionMode = m_selectionMode;
        toolBarParams.m_type = m_type;
        notifyObservers(toolBarParams);
    }

    ImGui::End();
}
