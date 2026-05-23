#pragma once
#include <string>
#include <stdexcept>
#include "Common.h"
#include "../Components/InputBox.h"

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
        static void render(ui::components::InputBox<T>* inputBox);

    private:
        static void initConfig(IConfig* config);

        static void destroy(IConfig* config);
    };

    template<typename T>
    void InputBox::render(ui::components::InputBox<T>*)
    {
        throw std::logic_error("Unsupported type for input box");
    }

    template<>
    void InputBox::render<int>(ui::components::InputBox<int>* inputBox)
    {
        initConfig(inputBox->config());

        ImGui::InputInt(inputBox->name().c_str(), inputBox->inputValue(), 0);

        destroy(inputBox->config());
    }

    template<>
    void InputBox::render<float>(ui::components::InputBox<float>* inputBox)
    {
        initConfig(inputBox->config());

        ImGui::InputFloat(inputBox->name().c_str(), inputBox->inputValue(), 0.0f);

        destroy(inputBox->config());
    }

    template<>
    void InputBox::render<double>(ui::components::InputBox<double>* inputBox)
    {
        initConfig(inputBox->config());

        ImGui::InputDouble(inputBox->name().c_str(), inputBox->inputValue(), 0.0);

        destroy(inputBox->config());
    }

    template<>
    void InputBox::render<char>(ui::components::InputBox<char>* inputBox)
    {
        initConfig(inputBox->config());

        static constexpr int bufferSize = 256;

        ImGui::InputText(inputBox->name().c_str(), inputBox->inputValue(), bufferSize, 0);

        destroy(inputBox->config());
    }

} // ui::styling