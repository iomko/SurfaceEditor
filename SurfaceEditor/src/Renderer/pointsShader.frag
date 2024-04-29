#version 330 core

in vec3 pointColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(pointColor, 1.0);
}
