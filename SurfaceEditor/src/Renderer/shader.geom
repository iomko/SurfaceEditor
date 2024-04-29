#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 color;

in DATA
{
	vec3 color;
	mat4 projectionMatrix;
} data_in[];

void main()
{
	gl_Position = data_in[0].projectionMatrix * gl_in[0].gl_Position;
	color = data_in[0].color;
	EmitVertex();

	gl_Position = data_in[1].projectionMatrix * gl_in[1].gl_Position;
	color = data_in[1].color;
	EmitVertex();

	gl_Position = data_in[2].projectionMatrix * gl_in[2].gl_Position;
	color = data_in[2].color;
	EmitVertex();

	EndPrimitive();
}