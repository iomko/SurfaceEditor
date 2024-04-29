#pragma once
#include <string>
#include "imgui/imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
import LayerSystem.Layer;


class AdditionLayer : public Layer, public Observable {
public:
    AdditionLayer(const std::string& name)
        : Layer(name)
    {
    }

    void onImGuiRender() override {
        ImGui::Begin(this->getName().c_str());

        if (ImGui::TreeNode("Add")) {
            if (ImGui::TreeNode("Mesh")) {
                if (ImGui::TreeNode("TriangulatedPlane")) {
                    ImGui::SliderInt("Subdivision", &m_subdivision, 1, 50);
                    //ImGui::SliderFloat("Size", &m_size, 1.0f, 100.0f);
                    ImGui::SliderFloat("Size", &m_size, 1.0f, 300.0f, "%.0f");

                    if (ImGui::Button("AddToScene")) {
                        AddPlaneCommand* addPlaneCommand = CommandRegistry::getCommand<AddPlaneCommand>();
                        addPlaneCommand->getPlaneProperties().m_subdivisionLevel = m_subdivision;
                        addPlaneCommand->getPlaneProperties().m_size = m_size;
                        addPlaneCommand->execute();
                    }

                    
                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        ImGui::End();
    }

private:
    int m_subdivision = 1;
    float m_size = 1.0f;
};
