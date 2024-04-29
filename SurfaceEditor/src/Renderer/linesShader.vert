#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in float aIsHighlighted;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

out vec3 lineColor;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(aPosition, 1.0f);
    
    if (aIsHighlighted == 1.0f) {
        lineColor = vec3(1.0f, 1.0f, 0.0f);
    } else {
        lineColor = vec3(54.0f / 255.0f, 54.0f / 255.0f, 54.0f / 255.0f);
    }
}