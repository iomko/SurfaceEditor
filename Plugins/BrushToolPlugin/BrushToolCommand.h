#pragma once
#include "../../src/Patterns/Observer.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Structures/Octree.h"

class BrushToolCommand : public Command<BrushToolParams, OctreeNodeDataParams>, public Observable
{
public:
	virtual void execute(const BrushToolParams& iParams, OctreeNodeDataParams& oParams) override;

	void undo() override;
};
