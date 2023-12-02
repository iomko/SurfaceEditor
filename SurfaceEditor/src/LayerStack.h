#ifndef LAYERSTACK_H
#define LAYERSTACK_H
#include <vector>

#include "Layer.h"

class LayerStack
{
public:
	void addLayer(Layer* layer)
	{
		layers.emplace(layers.begin() + lastLayerIndex, layer);
		++lastLayerIndex;
	}
	void removeLayer(Layer* layer)
	{
		auto it = std::find(layers.begin(), layers.end(), layer);
		if (it != layers.end())
		{
			layers.erase(it);
			--lastLayerIndex;
		}
	}

	void addOverlay(Layer* overlay)
	{
		layers.emplace_back(overlay);
	}
	void removeOverlay(Layer* overlay)
	{
		auto it = std::find(layers.begin(), layers.end(), overlay);
		if (it != layers.end())
		{
			layers.erase(it);
		}
	}
	~LayerStack()
	{
		for (auto layer : layers)
		{
			delete layer;
		}
	}
public:
	std::vector<Layer*>::iterator begin() { return layers.begin(); }
	std::vector<Layer*>::iterator end() { return layers.end(); }
private:
	std::vector<Layer*> layers;
	int lastLayerIndex = 0;
};
#endif