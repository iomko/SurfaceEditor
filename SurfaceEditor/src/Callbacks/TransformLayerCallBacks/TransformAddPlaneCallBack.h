#pragma once
#include "../Callback.h"
#include "../AddPlaneCallback.h"
#include "../../Gui/TransformLayer.h"

class TransformAddPlaneCallBack : public Callback
{
public:
	TransformAddPlaneCallBack(TransformLayer* transformLayer, AddPlaneCallback* addPlaneCallBack)
	{
		m_transformLayer = transformLayer;
		m_addPlaneCallBack = addPlaneCallBack;
	}

	virtual void execute() override
	{
		m_transformLayer->m_meshSettings.erase(m_addPlaneCallBack->m_addedPlane);

		// Insert the element into the map
		TransformLayer::TransformationSettings addedTransformationSettings;
		
		addedTransformationSettings.m_moveX = 0.0f;
		addedTransformationSettings.m_moveY = 0.0f;
		addedTransformationSettings.m_moveZ = 0.0f;

		m_transformLayer->m_meshSettings.insert({ m_addPlaneCallBack->m_addedPlane, addedTransformationSettings});
	}

private:
	TransformLayer* m_transformLayer;
	AddPlaneCallback* m_addPlaneCallBack;
};