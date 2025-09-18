#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"
#include "../ViewPortsController.h"
#include "UI/OutlinerLayer.h"

class MeshOutlinerAdderCallable : public Callable<MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override
	{

		Mesh* mesh = input.m_mesh;
        OutlinerLayer::addNode(0, "Mesh", mesh);
        //here I need to access the GuiOutlinerLayer class to be able to update that GuiOutlinerLayer
	}
};
