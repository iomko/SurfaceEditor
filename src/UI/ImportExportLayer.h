#pragma once
#include "../Core/Layer.h"
#include "../Patterns/Observer.h"

class ImportExportLayer : public Layer, public Observable
{
public:
    ImportExportLayer(const std::string& name);

	void onEvent(Event& event) override;

    void onImGuiRender() override;

private:    
    bool m_isMouseInsideWindow = false;
};
