#pragma once
import Renderer.Shader;

class ViewPortShaderSettings
{
public:
	Shader* m_meshShader = nullptr;
	Shader* m_normalsShader = nullptr;
	Shader* m_shaderSingleColor = nullptr;
	Shader* m_pointsShader = nullptr;
	Shader* m_linesShader = nullptr;
};
