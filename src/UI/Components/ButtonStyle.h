#pragma once
#include <imgui.h>
#include "RadioButton.h"

class ButtonStyle
{
public:
    ButtonStyle() = delete;

    ~ButtonStyle() = delete;

    static void setRadioButtonBackground(RadioButton* button, int& appliedColorStyles);

    static void controlPanelStyle(int& appliedColorStyles, int& appliedVarStyles);

    static void sidePanelStyle();

    static void popUpWindowStyle();

    static void closeStyling(int appliedColorStyles, int appliedVarStyles);
};