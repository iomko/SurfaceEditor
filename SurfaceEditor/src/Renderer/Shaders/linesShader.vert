#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in float aIsHighlighted;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

out vec3 v_lineColor;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(aPosition, 1.0f);

    if (aIsHighlighted == 1.0f) {
        v_lineColor = vec3(1.0f, 1.0f, 0.0f);  // yellow highlight
    } else {
        v_lineColor = vec3(54.0 / 255.0, 54.0 / 255.0, 54.0 / 255.0);  // dark gray
    }
}
