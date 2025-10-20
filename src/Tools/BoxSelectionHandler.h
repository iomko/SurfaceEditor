#pragma once
#include "InteractionHandler.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/BoxSelectionCommand.h"
#include "../Core/Input.h"

template<typename SelectCommandT>
class BoxSelectionHandler : public InteractionHandler<SelectCommandT>
{
public:
    BoxSelectionParams params;
	BoxSelectionHandler(SelectCommandT* command)
		: InteractionHandler<SelectCommandT>(command) {}

	void onBegin() override
	{
        glm::vec2 startPos = glm::vec2(Input::getMouseX(), Input::getMouseY());
        params.start_mouse_pos = startPos;
		this->getCommand()->execute(params);
	}
	void onUpdate() override 
    {
		this->getCommand()->execute(params);
    }
	void onEnd() override {}
};