#pragma once
#include <string>
#include <stdexcept>
#include <imgui.h>
#include "Common.h"

namespace ui::styling
{

    struct InputBoxConfig
    {
        float          width;
        ImVec4         background;
        AppliedStyling styles;
    };

    class InputBox
    {
    public:
        InputBox() = delete;

        ~InputBox() = delete;

        template<typename T>
        static void init(const std::string& name, T* input, InputBoxConfig& config);

        static void destroy(InputBoxConfig& config);

    private:
        static void initConfig(InputBoxConfig& config);
    };

    template<typename T>
    void InputBox::init(const std::string& name, T* input, InputBoxConfig& config)
    {
        throw std::logic_error("Unsupported type for input box");
    }

    template<>
    void InputBox::init<int>(const std::string& name, int* input, InputBoxConfig& config)
    {
        initConfig(config);

        ImGui::InputInt(name.c_str(), input, 0);
    }

    template<>
    void InputBox::init<float>(const std::string& name, float* input, InputBoxConfig& config)
    {
        initConfig(config);

        ImGui::InputFloat(name.c_str(), input, 0.0f);
    }

    template<>
    void InputBox::init<double>(const std::string& name, double* input, InputBoxConfig& config)
    {
        initConfig(config);

        ImGui::InputDouble(name.c_str(), input, 0.0);
    }

    template<>
    void InputBox::init<char>(const std::string& name, char* input, InputBoxConfig& config)
    {
        initConfig(config);

        static constexpr int bufferSize = 256;

        ImGui::InputText(name.c_str(), input, bufferSize, 0);
    }

} // ui::styling