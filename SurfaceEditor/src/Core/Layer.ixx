export module LayerSystem.Layer;
import EventSystem.Event;
import <string>;
import <vector>;

export class Layer
{
public:
	virtual ~Layer() {}
	virtual void onAttach() {}
	virtual void onDetach() {}
	virtual void onUpdate() {}
	virtual void onImGuiRender() {}
	virtual void onEvent(Event& event) {}
	std::string getName();
	Layer(std::string name);
private:
	std::string m_name;
};

export class LayerStack
{
public:
	void addLayer(Layer* layer);
	void removeLayer(Layer* layer);
	void addOverlay(Layer* overlay);
	void removeOverlay(Layer* overlay);
	std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
	std::vector<Layer*>::iterator end() { return m_layers.end(); }
private:
	std::vector<Layer*> m_layers;
	int m_lastLayerIndex = 0;
};
