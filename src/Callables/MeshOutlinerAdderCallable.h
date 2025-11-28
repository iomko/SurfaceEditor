#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"
#include "../ViewPortsController.h"
#include "UI/OutlinerService.h"

class MeshOutlinerAdderCallable : public Callable<MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override
	{
		Mesh* mesh = input.m_mesh;
        Outliner::addNode(0, "Mesh", mesh);
	}
};
