#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in DATA
{
	vec3 g_normal;
	mat4 g_projectionMatrix;
} data_in[];

void main()
{
	gl_Position = data_in[0].g_projectionMatrix * gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = data_in[0].g_projectionMatrix * (gl_in[0].gl_Position + 1.0f * vec4(data_in[0].g_normal, 0.0f));
	EmitVertex();
	EndPrimitive();

	gl_Position = data_in[1].g_projectionMatrix * gl_in[1].gl_Position;
	EmitVertex();
	gl_Position = data_in[1].g_projectionMatrix * (gl_in[1].gl_Position + 1.0f * vec4(data_in[1].g_normal, 0.0f));
	EmitVertex();
	EndPrimitive();

	gl_Position = data_in[2].g_projectionMatrix * gl_in[2].gl_Position;
	EmitVertex();
	gl_Position = data_in[2].g_projectionMatrix * (gl_in[2].gl_Position + 1.0f * vec4(data_in[2].g_normal, 0.0f));
	EmitVertex();
	EndPrimitive();
}


