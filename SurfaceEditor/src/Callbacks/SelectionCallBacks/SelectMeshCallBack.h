#pragma once
#include <iostream>

#include "../Callback.h"
#include "../ViewPortsHolder.h"
#include "../AABBBoundingRegion.h"
#include "../Core/Input.h"

class SelectMeshCallBack : public Callback, public Observer
{
public:
	virtual void execute() override
	{
		//select Mesh
	}
public:
    Mesh* m_selectedMesh = nullptr;
};
