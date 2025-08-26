#pragma once
#include <Renderer/Shader.h>

class ViewPortLayerRenderSettings
{
public:
	Shader* m_meshShader = nullptr;
	Shader* m_edgeShader = nullptr;
	Shader* m_faceShader = nullptr;
};
