#pragma once
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"
#include "../Structures/Octree.h"
#include "CommandIDs.h"

class BrushToolCommand : public Command<BRUSH_TOOL_COMMAND ,BrushToolParams, OctreeNodeDataParams>, public Observable
{
public:
	virtual void execute(const BrushToolParams& iParams, OctreeNodeDataParams& oParams) override;

	void undo() override;
};
