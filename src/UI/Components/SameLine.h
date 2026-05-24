#pragma once
#include <imgui.h>
#include "Common.h"

namespace ui::components
{

    class SameLine : public IComponent
    {
    public:
        SameLine()
            : IComponent({}, {})
        { }

        void render() override
        {
            ImGui::SameLine();
        }
    };

} // ui::components
