#pragma once
#include "../Core/Layer.h"
#include "../Patterns/Observer.h"
#include "LayerIDs.h"
#include "LayerRegistry.h"

class ImportExportLayer : public LayerWithID<IMPORT_EXPORT_LAYER>, public Observable
{
public:
    ImportExportLayer(const std::string& name);

	void onEvent(Event& event) override;

    void onImGuiRender() override;

private:    
    bool m_isMouseInsideWindow = false;
};
