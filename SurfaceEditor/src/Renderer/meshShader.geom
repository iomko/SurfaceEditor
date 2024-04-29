#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 g_color;

in DATA
{
	vec3 g_color;
	vec3 g_normal;
	mat4 g_projectionMatrix;
} data_in[];

void main()
{
	gl_Position = data_in[0].g_projectionMatrix * gl_in[0].gl_Position;
	g_color = data_in[0].g_color;
	EmitVertex();

	gl_Position = data_in[1].g_projectionMatrix * gl_in[1].gl_Position;
	g_color = data_in[1].g_color;
	EmitVertex();

	gl_Position = data_in[2].g_projectionMatrix * gl_in[2].gl_Position;
	g_color = data_in[2].g_color;
	EmitVertex();

	EndPrimitive();
}