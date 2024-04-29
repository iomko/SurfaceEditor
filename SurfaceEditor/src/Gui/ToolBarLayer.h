#pragma once
#include <string>
#include "imgui/imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
import LayerSystem.Layer;

class ToolBarLayer : public Layer, public Observable {
public:
    ToolBarLayer(ViewPortHolder* viewPortHolder, const std::string& name)
        : Layer(name)
    {
        addObserver(viewPortHolder);
    }

    enum SelectionMode {
        Face,
        Edge,
        Vertex,
        Object
    };

    enum Type {
        Selection,
        Deselection
    };

    SelectionMode getSelectionMode() const {
        return m_selectionMode;
    }

    Type getType() const {
        return m_type;
    }

    void onImGuiRender() override {
        ImGui::Begin(this->getName().c_str());

        if (ImGui::TreeNode("Selection Mode")) {
            int selectionModeInt = static_cast<int>(m_selectionMode);
            if (ImGui::RadioButton("Face Mode", &selectionModeInt, SelectionMode::Face) ||
                ImGui::RadioButton("Edge Mode", &selectionModeInt, SelectionMode::Edge) ||
                ImGui::RadioButton("Vertex Mode", &selectionModeInt, SelectionMode::Vertex) ||
                ImGui::RadioButton("Object Mode", &selectionModeInt, SelectionMode::Object)) {
                m_selectionMode = static_cast<SelectionMode>(selectionModeInt);
                notifyObservers();
            }
            ImGui::TreePop();
        }

        int typeInt = static_cast<int>(m_type);
        if (ImGui::RadioButton("Selection", &typeInt, Type::Selection) || 
            ImGui::RadioButton("Deselection", &typeInt, Type::Deselection)) {
            m_type = static_cast<Type>(typeInt);
            notifyObservers();
        }

        ImGui::End();
    }

private:
    SelectionMode m_selectionMode = SelectionMode::Face;
    Type m_type = Type::Selection;
};