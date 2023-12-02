#ifndef UIMANAGER_H
#define UIMANAGER_H
#include "Layer.h"
#include "ToolBarLayer.h"

class UIManagerLayer : Layer
{
	std::vector<Layer*> layers;
public:
	~UIManagerLayer() override;
	void onAttach() override
	{
		//creating layers and attaching them
		ToolBarLayer* toolBarLayer = new ToolBarLayer;
		toolBarLayer->onAttach();
		layers.push_back(toolBarLayer);
	}

	void setToolBarCallBack()
	{
		toolBarLayer
	}

	void setOutlinerCallBack()
	{
		
	}

	void onDetach() override;
	void onUpdate() override;
	void onImGuiRender() override
	{
		for(auto layer : layers)
		{
			layer->onImGuiRender();
		}
	}
	void onEvent(Event& event) override
	{
		for (auto layer : layers)
		{
			layer->onEvent(event);
		}
	}
};

#endif
