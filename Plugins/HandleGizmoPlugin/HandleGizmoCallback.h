#pragma once
#include <functional>
#include <ImGuizmo.h>
#include "CallbackIDs.h"
#include "Callback.h"
#include "../Params/OperationParams.h"
#include "../Patterns/Observer.h"

enum class SelectionMode;

class HandleGizmoCallBack : public Callback<GizmoParams>, public Observer
{
public:
    HandleGizmoCallBack();

	void execute(const GizmoParams& iParams) override;

private:
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

    // void chooseSelectionMode(const GizmoParams& iParams, bool& end);

    void init(bool& earlyReturn);

    void update();

    void handleGizmo(ImGuizmo::OPERATION operation, std::function<void()> executeRealTime);

private:
    glm::mat4 m_realTimeTransform = glm::mat4(1.0f);
    glm::mat4 m_gizmoTransform = glm::mat4(1.0f);
    glm::mat4 m_transform = glm::mat4(1.0f);
    
    SelectionMode m_selectionMode;
};
