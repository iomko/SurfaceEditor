#pragma once
#include "../../src/Core/Layer.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/UI/LayerRegistry.h"
#include <Components/Window.h>
#include <Components/Button.h>
#include <Components/CheckBox.h>

class ImportExportLayer : public Layer, public Observable, public ui::components::Window
{
public:
    ImportExportLayer(const std::string& name);

	void onEvent(Event& event) override;

    void onImGuiRender() override;

protected:

    void initWindowConfig() override;

    void initComponents() override;


private:    
    bool m_isMouseInsideWindow = false;
    bool importClicked = false;
    bool exportClicked = false;
    bool onlySelected = false;
    bool isMouseOnHeader = false;

private:
    ui::components::Button* m_importButton;
    ui::components::Button* m_exportButton;
    ui::components::CheckBox* m_exportSelectedCheckBox;
};
