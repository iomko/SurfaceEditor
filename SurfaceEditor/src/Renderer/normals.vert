#version 330 core
layout (location = 0) in vec3 l_Pos;
layout (location = 1) in vec3 l_Normal;


out DATA
{
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



}










