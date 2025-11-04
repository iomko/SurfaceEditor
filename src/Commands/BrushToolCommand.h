#pragma once
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"
#include "../Structures/Octree.h"

class BrushToolCommand : public Command<BrushToolParams, OctreeNodeDataParams>, public Observable
{
public:
	virtual void execute(const BrushToolParams& iParams, OctreeNodeDataParams& oParams) override;

	void undo() override;
};
