#pragma once
#include <functional>
#include "ImGuizmo.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Callbacks/Callback.h"

class HandleGizmoCallBack : public Callback<GizmoParams>, public Observer
{
public:
    HandleGizmoCallBack();

    glm::vec3 calcFaceMiddlePos(ExtendedFace* face);

    template<typename T>
    void calcMiddlePos(const std::vector<T>& selectedItems, std::function<glm::vec3(T)> positionCallback)
    {
        glm::vec3 sum(0.0f);

        for (const auto& item : selectedItems)
        {
            item->m_realTimeTransform = &m_realTimeTransform;
            sum += positionCallback(item);
        }

        glm::vec3 center = sum / float(selectedItems.size());

        m_gizmoTransform = glm::mat4(1.0f);
        m_gizmoTransform[3] = glm::vec4(center, 1.0f);
    }

    void chooseSelectionMode(const GizmoParams& iParams, bool& end);

    void update();

    void handleGizmo(ImGuizmo::OPERATION operation, std::function<void()> executeRealTime);

	void execute(const GizmoParams& iParams) override;

private:
    glm::mat4 m_realTimeTransform = glm::mat4(1.0f);
    glm::mat4 m_gizmoTransform = glm::mat4(1.0f);
    glm::mat4 m_transform = glm::mat4(1.0f);
    
    int m_lastSelectedMeshesCount = 0;
    int m_lastSelectedFacesCount = 0;

    GizmoParams::SelectionMode m_selectionMode;
};
