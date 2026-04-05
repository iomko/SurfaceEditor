#pragma once
#include "../../src/Core/Layer.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/UI/LayerRegistry.h"

class ImportExportLayer : public Layer, public Observable
{
public:
    ImportExportLayer(const std::string& name);

	void onEvent(Event& event) override;

    void onImGuiRender() override;

private:    
    bool m_isMouseInsideWindow = false;
};
