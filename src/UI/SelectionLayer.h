#pragma once
#include <string>
#include "imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Core/Layer.h"

class SelectionLayer : public Layer, public Observable {
public:
    SelectionLayer(const std::string& name)
        : Layer(name)
    {}

    SelectionLayerParams::SelectionMode getSelectionMode() const {
        return m_selectionMode;
    }

    SelectionLayerParams::Type getType() const {
        return m_type;
    }

    void onImGuiRender() override {
        ImGui::Begin(this->getName().c_str());
        if (ImGui::Button("Box Selection Mode")) {
            m_selectionMode = SelectionLayerParams::SelectionMode::BoxSelection;

            SelectionLayerParams toolBarParams;
            toolBarParams.m_selectionMode = m_selectionMode;
            toolBarParams.m_type = SelectionLayerParams::Type::Selection;
            notifyObservers(toolBarParams);
        }
        if (ImGui::TreeNode("Selection Mode")) {
            int selectionModeInt = static_cast<int>(m_selectionMode);
            if (ImGui::RadioButton("Face Mode", &selectionModeInt, SelectionLayerParams::SelectionMode::Face) ||
                ImGui::RadioButton("Edge Mode", &selectionModeInt, SelectionLayerParams::SelectionMode::Edge) ||
                ImGui::RadioButton("Vertex Mode", &selectionModeInt, SelectionLayerParams::SelectionMode::Vertex) ||
                ImGui::RadioButton("Object Mode", &selectionModeInt, SelectionLayerParams::SelectionMode::Object)) {
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
            ImGui::RadioButton("Deselection", &typeInt, SelectionLayerParams::Type::Deselection)) {
            m_type = static_cast<SelectionLayerParams::Type>(typeInt);

			SelectionLayerParams toolBarParams;
			toolBarParams.m_selectionMode = m_selectionMode;
			toolBarParams.m_type = m_type;
            notifyObservers(toolBarParams);
        }

        ImGui::End();
    }

private:
    SelectionLayerParams::SelectionMode m_selectionMode = SelectionLayerParams::SelectionMode::Face;
    SelectionLayerParams::Type m_type = SelectionLayerParams::Type::Selection;
};
