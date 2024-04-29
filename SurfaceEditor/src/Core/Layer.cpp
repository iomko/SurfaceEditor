module LayerSystem.Layer;

std::string Layer::getName() { return m_name; }

Layer::Layer(std::string name)
{
	m_name = name;
}

void LayerStack::addLayer(Layer* layer)
{
	m_layers.emplace(m_layers.begin() + m_lastLayerIndex, layer);
	++m_lastLayerIndex;
}

void LayerStack::removeLayer(Layer* layer)
{
	const auto& layerIt = std::find(m_layers.begin(), m_layers.end(), layer);
	if (layerIt != m_layers.end())
	{
		m_layers.erase(layerIt);
		--m_lastLayerIndex;
	}
}

void LayerStack::addOverlay(Layer* overlay)
{
	m_layers.emplace_back(overlay);
}

void LayerStack::removeOverlay(Layer* overlay)
{
	const auto& layersIt = std::find(m_layers.begin(), m_layers.end(), overlay);
	if (layersIt != m_layers.end())
	{
		m_layers.erase(layersIt);
	}
}