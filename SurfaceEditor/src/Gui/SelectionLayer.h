#pragma once
#include <string>
#include "imgui/imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "Layer.h"

class SelectionLayer : public Layer, public Observable {
public:
    SelectionLayer(const std::string& name)
        : Layer(name)
    {}

    ToolBarParams::SelectionMode getSelectionMode() const {
        return m_selectionMode;
    }

    ToolBarParams::Type getType() const {
        return m_type;
    }

    void onImGuiRender() override {
        ImGui::Begin(this->getName().c_str());

        if (ImGui::TreeNode("Selection Mode")) {
            int selectionModeInt = static_cast<int>(m_selectionMode);
            if (ImGui::RadioButton("Face Mode", &selectionModeInt, ToolBarParams::SelectionMode::Face) ||
                ImGui::RadioButton("Edge Mode", &selectionModeInt, ToolBarParams::SelectionMode::Edge) ||
                ImGui::RadioButton("Vertex Mode", &selectionModeInt, ToolBarParams::SelectionMode::Vertex) ||
                ImGui::RadioButton("Object Mode", &selectionModeInt, ToolBarParams::SelectionMode::Object)) {
                m_selectionMode = static_cast<ToolBarParams::SelectionMode>(selectionModeInt);

                ToolBarParams toolBarParams;
                toolBarParams.m_selectionMode = m_selectionMode;
                toolBarParams.m_type = m_type;
                notifyObservers(toolBarParams);
            }
            ImGui::TreePop();
        }

        int typeInt = static_cast<int>(m_type);
        if (ImGui::RadioButton("Selection", &typeInt, ToolBarParams::Type::Selection) ||
            ImGui::RadioButton("Deselection", &typeInt, ToolBarParams::Type::Deselection)) {
            m_type = static_cast<ToolBarParams::Type>(typeInt);

			ToolBarParams toolBarParams;
			toolBarParams.m_selectionMode = m_selectionMode;
			toolBarParams.m_type = m_type;
            notifyObservers(toolBarParams);
        }

        ImGui::End();
    }

private:
    ToolBarParams::SelectionMode m_selectionMode = ToolBarParams::SelectionMode::Face;
    ToolBarParams::Type m_type = ToolBarParams::Type::Selection;
};