#version 330 core
layout (location = 0) in vec3 l_Pos;
layout (location = 1) in vec3 l_Normal;
layout(location = 2) in float l_IsHighlighted;

out DATA
{
	vec3 g_color;
	vec3 g_normal;
	mat4 g_projectionMatrix;
} data_out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{

	gl_Position = vec4(l_Pos, 1.0f);

	data_out.g_normal = l_Normal;

	data_out.g_projectionMatrix = u_projection * u_view * u_model;
	
	if (l_IsHighlighted == 1.0f)
		data_out.g_color = vec3(255.0f / 255.0f, 198.0f / 255.0f, 111.0f / 255.0f);
	else
		data_out.g_color = vec3(0.5, 0.5, 0.5);

}