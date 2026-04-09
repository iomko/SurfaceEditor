#pragma once
#include <algorithm>
#include <imgui.h>
#include "RadioButton.h"

struct ButtonLayout
{
    float width;
    float height;
};

class ButtonStyle
{
public:
    ButtonStyle() = delete;

    ~ButtonStyle() = delete;

    static void setRadioButtonBackground(RadioButton* button, int& appliedColorStyles);

    static void controlPanelStyle(int& appliedColorStyles, int& appliedVarStyles);

    static void sidePanelStyle();

    static void simplePopUpWindowStyle(float buttonHeight, int& appliedColorStyles, int& appliedVarStyles);

    static void applyResponsiveFontScale(float buttonHeight, float baseFontSize = 16.0f);

    static ButtonLayout calculateVerticalButtonLayout(int buttonCount);

    static void closeStyling(int appliedColorStyles, int appliedVarStyles);
};