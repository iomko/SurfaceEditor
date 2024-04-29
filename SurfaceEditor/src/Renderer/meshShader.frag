#version 330 core
out vec4 FragColor;
in vec3 g_color;
void main()
{
   FragColor = vec4(g_color,1.0);
}