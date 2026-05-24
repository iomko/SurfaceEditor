#pragma once
#include "Common.h"

namespace ui::components
{

    class Separator : public IComponent
    {
    public:
        Separator(float length)
            : IComponent({}, {})
            , m_length(length)
        { }

        void render() override
        {
            ImVec2 pos             = ImGui::GetCursorScreenPos();
            ImVec2 windowSize = ImGui::GetWindowSize();
            float responsiveLenght = windowSize.x * m_length;

            ImGui::GetWindowDrawList()->AddLine(
                pos,
                ImVec2(pos.x + responsiveLenght, pos.y),
                ImGui::GetColorU32(ImGuiCol_Separator),
                1.0f);
        }

    private:
        float m_length;
    };

} // ui::components