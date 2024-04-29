#pragma once
#include "../Callback.h"
#include "../SelectionCallBacks/SelectMeshCallBack.h"
#include "../../Gui/TransformLayer.h"

class TransformSelectMeshCallBack : public Callback
{
public:
	TransformSelectMeshCallBack(TransformLayer* transformLayer, SelectMeshCallBack* selectMeshCallBack)
	{
		m_transformLayer = transformLayer;
		m_selectMeshCallBack = selectMeshCallBack;
	}

	virtual void execute() override
	{
		Mesh* selectedMesh = m_selectMeshCallBack->m_selectedMesh;
		m_transformLayer->m_lastSelectedMesh = selectedMesh;

		m_transformLayer->updateFromCallBack();
	}

private:
	TransformLayer* m_transformLayer;
	SelectMeshCallBack* m_selectMeshCallBack;
};