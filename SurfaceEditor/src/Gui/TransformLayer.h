#pragma once
#include <string>
#include "imgui/imgui.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Callbacks/ApplyTransformationMatrix.CallBack.h"
#include "imgui/imgui_internal.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/TransformMeshCommand.h"
import LayerSystem.Layer;

class TransformLayer : public Layer, public Observer {
public:

    struct TransformationSettings
    {
        float m_moveX = 0.0f;
        float m_moveY = 0.0f;
        float m_moveZ = 0.0f;
    };

    TransformLayer(ViewPortHolder* viewPortHolder, const std::string& name)
        : Layer(name), m_transformationMatrixCallBack(viewPortHolder),
        m_moveX(0.0f),
        m_moveY(0.0f),
        m_moveZ(0.0f)
    {
    }

    virtual void onImGuiRender() override {
        ImGui::Begin(this->getName().c_str());

        bool meshSelected = (m_lastSelectedMesh != nullptr);

        // Move X slider
        ImGui::Text("Move X");
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !meshSelected);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, meshSelected ? 1.0f : 0.5f);
        if (ImGui::DragFloat("##MoveXSlider", &m_moveX, m_speed, -FLT_MAX, FLT_MAX)) {
            updateTransformationMatrix();
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {

            TransformMeshCommand* transformMeshCommand = CommandRegistry::getCommand<TransformMeshCommand>();
            transformMeshCommand->m_transformedMesh = m_lastSelectedMesh;
            transformMeshCommand->m_modelMatrix = m_transformationMatrixCallBack.m_modelMatrix;
            transformMeshCommand->execute();
            std::cout << "Move X Slider Clicked Outside" << std::endl;
        }


        if (ImGui::IsItemHovered()) {
            adjustSpeed();
        }
        ImGui::PopStyleVar();
        ImGui::PopItemFlag();

        // Move Y slider
        ImGui::Text("Move Y");
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !meshSelected);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, meshSelected ? 1.0f : 0.5f);
        if (ImGui::DragFloat("##MoveYSlider", &m_moveY, m_speed, -FLT_MAX, FLT_MAX)) {
            updateTransformationMatrix();
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            std::cout << "Move Y Slider Clicked Outside" << std::endl;
        }


        if (ImGui::IsItemHovered()) {
            adjustSpeed();
        }
        ImGui::PopStyleVar();
        ImGui::PopItemFlag();

        // Move Z slider
        ImGui::Text("Move Z");
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !meshSelected);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, meshSelected ? 1.0f : 0.5f);
        if (ImGui::DragFloat("##MoveZSlider", &m_moveZ, m_speed, -FLT_MAX, FLT_MAX)) {
            updateTransformationMatrix();
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            std::cout << "Move Z Slider Clicked Outside" << std::endl;
        }


        if (ImGui::IsItemHovered()) {
            adjustSpeed();
        }
        ImGui::PopStyleVar();
        ImGui::PopItemFlag();

        // Apply button
        if (ImGui::Button("Apply")) {
            // Handle Apply button click
            // Notify observers
            //notifyObservers();
        }

        // Cancel button
        if (ImGui::Button("Cancel")) {
            // Handle Cancel button click
            // Notify observers
            //notifyObservers();
        }

        ImGui::End();
    }

    float getMoveX() const {
        return m_moveX;
    }

    float getMoveY() const {
        return m_moveY;
    }

    float getMoveZ() const {
        return m_moveZ;
    }

public:
    float m_moveX = 0.0f;
    float m_moveY = 0.0f;
    float m_moveZ = 0.0f;
    float m_speed = 1.0f; // Initial speed for sliders

    //callbacks
    ApplyTransformationCallBack m_transformationMatrixCallBack;

    void adjustSpeed() {
        float scroll = ImGui::GetIO().MouseWheel;
        if (scroll != 0.0f) {
            m_speed += scroll * 10.0f; // Adjust the speed based on mouse wheel movement
            m_speed = std::max(m_speed, 1.0f); // Ensure speed is not negative
        }
    }

    void updateFromCallBack()
    {
        if (m_lastSelectedMesh != nullptr) {
            auto it = m_meshSettings.find(m_lastSelectedMesh);

            if(it != m_meshSettings.end())
            {
	            m_moveX = it->second.m_moveX;
				m_moveY = it->second.m_moveY;
				m_moveZ = it->second.m_moveZ;
            }
        }
    }


    void updateTransformationMatrix() {
        // If m_lastSelectedMesh is nullptr, set the values to infinity or don't update at all

        if (m_lastSelectedMesh != nullptr) {
            // If m_lastSelectedMesh is not nullptr, update the values from m_meshSettings
            auto it = m_meshSettings.find(m_lastSelectedMesh);
            if (it != m_meshSettings.end()) {
                it->second.m_moveX = m_moveX;
                it->second.m_moveY = m_moveY;
                it->second.m_moveZ = m_moveZ;

                std::cout << "moveX: " << it->second.m_moveX << " moveY: " << it->second.m_moveY << " moveZ: " << it->second.m_moveZ << std::endl;


                m_moveX = it->second.m_moveX;
                m_moveY = it->second.m_moveY;
                m_moveZ = it->second.m_moveZ;
            }
        }

        m_transformationMatrixCallBack.m_currentMesh = m_lastSelectedMesh;
        m_transformationMatrixCallBack.setTranslationVector({ m_moveX, m_moveY, m_moveZ });
        m_transformationMatrixCallBack.execute();
    }


public:
    std::map<Mesh*, TransformationSettings> m_meshSettings;
    Mesh* m_lastSelectedMesh = nullptr;
};
