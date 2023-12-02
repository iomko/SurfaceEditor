#ifndef VIEWPORTLAYER_H
#define VIEWPORTLAYER_H

#include "Layer.h"
#include "ToolBarLayer.h"
#include "Scene.h"

class ViewPortLayer : Layer
{
	ViewPortLayer(Scene* scene,ToolBarLayer* toolBarLayer)
	{
		this->currentTransformationTool = toolBarLayer->getToolBarToolState();
		this->currentSelectionMode = toolBarLayer->getToolBarModeState();
		this->toolBarLayer = toolBarLayer;
		this->scene = scene;
	}
	void onEvent(Event& event) override
	{
		
	}

	void executeSelection()
	{
		if(this->currentSelectionMode == ToolBarMode::ObjectMode)
		{
			
			for (auto& pair : this->scene->meshOctreePairs) {
				const Mesh& mesh = pair.first;
				
				
			}

		} else if(this->currentSelectionMode == ToolBarMode::PolygonMode)
		{
			
		}
	}

	void setToolBarCallBack()
	{
		toolBarLayer->addCallback(std::bind(&ViewPortLayer::setSelectionTool, this, std::placeholders::_1));
		toolBarLayer->addCallback(std::bind(&ViewPortLayer::setTransformationTool, this, std::placeholders::_1));
	}

	void setSelectionTool()
	{
		toolBarLayer->getToolBarToolState();
	}

	void setTransformationTool()
	{
		toolBarLayer->getToolBarModeState();
	}
private:
	ToolBarTool currentTransformationTool;
	ToolBarMode currentSelectionMode;
	ToolBarLayer* toolBarLayer = nullptr;
	Scene* scene = nullptr;
};

#endif