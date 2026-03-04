#pragma once
#include "../../src/Patterns/Observer.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Structures/Octree.h"
#include "../../src/Commands/CommandIDs.h"

class BrushToolCommand : public Command<BRUSH_TOOL_COMMAND ,BrushToolParams, OctreeNodeDataParams>, public Observable
{
public:
	virtual void execute(const BrushToolParams& iParams, OctreeNodeDataParams& oParams) override;

	void undo() override;
};
