#pragma once

#include "../Core/Layer.h"

class ImGuiLayer : public Layer
{
public:
	explicit ImGuiLayer(const std::string& name)
		: Layer(name)
	{
	}
	void onAttach() override;
	void onDetach() override;
	void begin();
	void end();
};
