#pragma once
#include "../../src/Core/Layer.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/UI/LayerIDs.h"
#include "../../src/UI/LayerRegistry.h"

class ImportExportLayer : public LayerWithID<IMPORT_EXPORT_LAYER>, public Observable
{
public:
    ImportExportLayer(const std::string& name);

	void onEvent(Event& event) override;

    void onImGuiRender() override;

private:    
    bool m_isMouseInsideWindow = false;
};
