#pragma once
#include <string>
#include <stdexcept>
#include "Common.h"

namespace ui::styling
{

    struct InputBoxConfig : IConfig
    {
        float  width;
        ImVec4 background;
    };

    class InputBox
    {
    public:
        InputBox() = delete;

        template<typename T>
        static void render(const std::string& name, T* value, IConfig* config);

    private:
        static void initConfig(IConfig* config);

        static void destroy(IConfig* config);
    };

    template<typename T>
    void InputBox::render(const std::string&, T*, IConfig*)
    {
        throw std::logic_error("Unsupported type for input box");
    }

    template<>
    void InputBox::render<int>(const std::string& name, int* value, IConfig* config)
    {
        initConfig(config);

        ImGui::InputInt(name.c_str(), value, 0);

        destroy(config);
    }

    template<>
    void InputBox::render<float>(const std::string& name, float* value, IConfig* config)
    {
        initConfig(config);

        ImGui::InputFloat(name.c_str(), value, 0.0f);

        destroy(config);
    }

    template<>
    void InputBox::render<double>(const std::string& name, double* value, IConfig* config)
    {
        initConfig(config);

        ImGui::InputDouble(name.c_str(), value, 0.0);

        destroy(config);
    }

    template<>
    void InputBox::render<char>(const std::string& name, char* value, IConfig* config)
    {
        initConfig(config);

        static constexpr int bufferSize = 256;

        ImGui::InputText(name.c_str(), value, bufferSize, 0);

        destroy(config);
    }

} // ui::styling
