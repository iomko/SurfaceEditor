#pragma once
#include <string>
#include <variant>
#include "../src/Patterns/Observer.h"
#include "../src/Core/Layer.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/OverlappingWindow.h"

namespace ui::components
{
    class Label;
    class CheckBox;

    template<typename T>
    class InputBox;
} // ui::components

enum class AdditionType
{
	NONE 	= -1,
	PLANE 	= 0,
	CUBE	= 1,
	SURFACE	= 2
};

class AdditionLayer : public Layer, public Observable, public Observer, public OverlappingWindow {
public:
    using CommandParams = std::variant<PlaneParams, CubeParams>;

    AdditionLayer(const std::string& name);

	void onImGuiRender() override;

    void setAdditionType(const AdditionType& additionType)
    {
        m_additionType = additionType;
    }

protected:
    ui::styling::WindowConfig initWindowConfig() override;

    void initComponents() override;

private:
	void addPlane();

	void addCube();

	void addSurface();

	void drawMeshComponents(CommandConcept* command, std::function<CommandParams()> paramsCallback);

    void drawSurfaceComponents(CommandConcept* command);

private:
    static constexpr int API_BUFFER_SIZE = 256;

	AdditionType m_additionType;

    // // Surface inputs
	// float m_lowerLeftLon{};
	// float m_lowerLeftLat{};
	// float m_upperRightLon{};
	// float m_upperRightLat{};
	// char m_apiKeyBuffer[API_BUFFER_SIZE];

    // Mesh components
    std::unique_ptr<ui::components::Label>           m_subdivisionLabel;
    std::unique_ptr<ui::components::Label>           m_sizeLabel;
    std::unique_ptr<ui::components::Label>           m_positionLabel;
    std::unique_ptr<ui::components::Label>           m_automaticLabel;
    std::unique_ptr<ui::components::Label>           m_posXLabel;
    std::unique_ptr<ui::components::Label>           m_posYLabel;
    std::unique_ptr<ui::components::Label>           m_posZLabel;
    std::unique_ptr<ui::components::CheckBox>        m_automaticCheckbox;
    std::unique_ptr<ui::components::InputBox<int>>   m_subdivisionInputBox;
    std::unique_ptr<ui::components::InputBox<float>> m_sizeInputBox;
    std::unique_ptr<ui::components::InputBox<float>> m_posXInputBox;
    std::unique_ptr<ui::components::InputBox<float>> m_posYInputBox;
    std::unique_ptr<ui::components::InputBox<float>> m_posZInputBox;

    // Surface components
    //TODO
};
