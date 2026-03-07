#pragma once

#include "../Core/Layer.h"
#include "LayerRegistry.h"
#include "LayerIDs.h"
#include "Components/FontStyle.h"

class ImGuiLayer : public LayerWithID<IM_GUI_LAYER>
{
public:
	explicit ImGuiLayer(const std::string& name)
		: LayerWithID(name) {}
	void onAttach() override;
	void onDetach() override;
	void begin();
	void end();
};
