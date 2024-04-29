#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
//out vec3 vertexColor;

out DATA
{
	vec3 color;
	mat4 projectionMatrix;
} data_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//gl_Position = projection * view * model * vec4(aPos, 1.0f);
	gl_Position = vec4(aPos, 1.0f);
	data_out.color = aColor;
	data_out.projectionMatrix = projection * view * model;
	//vertexColor = aColor;

}