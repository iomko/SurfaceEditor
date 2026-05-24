// #pragma once
// #include <string>
// #include <variant>
// #include "../src/Patterns/Observer.h"
// #include "../src/Core/Layer.h"
// #include "../src/UI/LayerRegistry.h"
// #include "../src/UI/IWindow.h"

// namespace ui::components
// {
//     class Button;
//     class Label;
//     class CheckBox;

//     template<typename T>
//     class InputBox;

//     template<typename T>
//     class Slider;
// } // ui::components

// enum class AdditionType
// {
// 	NONE 	= -1,
// 	PLANE 	= 0,
// 	CUBE	= 1,
// 	SURFACE	= 2
// };

// class AdditionLayer : public Layer, public Observable, public Observer, public IWindow
// {
// public:
//     using CommandParams = std::variant<PlaneParams, CubeParams>;

//     AdditionLayer(const std::string& name);

// 	void onImGuiRender() override;

//     void setAdditionType(const AdditionType& additionType)
//     {
//         m_additionType = additionType;
//     }

// protected:
//     ui::styling::WindowConfig initWindowConfig() override;

//     void initComponents() override;

// private:
// 	void addPlane();

// 	void addCube();

// 	void addSurface();

//     void drawDialogComponents();

//     void drawMeshComponents(CommandConcept* command, std::function<CommandParams()> paramsCallback);

//     void drawSurfaceComponents(CommandConcept* command);

// private:
// 	AdditionType m_additionType;

//     // Surface components
//     //TODO
//     // static constexpr int API_BUFFER_SIZE = 256;
//     // float m_lowerLeftLon{};
// 	// float m_lowerLeftLat{};
// 	// float m_upperRightLon{};
// 	// float m_upperRightLat{};
// 	// char m_apiKeyBuffer[API_BUFFER_SIZE];
// };
