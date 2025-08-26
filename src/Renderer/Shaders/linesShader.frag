#version 330 core

in vec3 v_lineColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(v_lineColor, 1.0);
}