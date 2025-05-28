#pragma once
import Renderer.Shader;

class ViewPortLayerRenderSettings
{
public:
	Shader* m_meshShader = nullptr;
	Shader* m_edgeShader = nullptr;
	Shader* m_faceShader = nullptr;
};
