export module LayerSystem.Layer.ImGuiLayer;
import LayerSystem.Layer;

export class ImGuiLayer : public Layer
{
public:
	explicit ImGuiLayer(const std::string& name)
		: Layer(name)
	{
	}
	void onAttach() override;
	void onDetach() override;
	void begin();
	void end();
};
