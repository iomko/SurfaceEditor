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
    } else if(aIsHighlighted == 0.0f) {
        v_lineColor = vec3(54.0 / 255.0, 54.0 / 255.0, 54.0 / 255.0);  // dark gray
    } else if(aIsHighlighted == 0.5f) {
        v_lineColor = vec3(0.9f, 0.1f, 0.3f);
    } else if(aIsHighlighted == 0.25f) {
        v_lineColor = vec3(0.1f, 0.0f, 0.6f); //modra
    } else if(aIsHighlighted == 0.1f) {
        v_lineColor = vec3(1.0f, 0.0f, 0.0f); //cervena
    } else if(aIsHighlighted == 0.4f) {
        v_lineColor = vec3(0.0f, 1.0f, 0.0f); //faceNormal
    } else if(aIsHighlighted == 0.11f) {
        v_lineColor = vec3(0.035f, 0.886f, 0.902f); //planeNormal
    } else if(aIsHighlighted == 0.15f) {
        v_lineColor = vec3(1.0f, 0.0f, 0.749f); //perpendicularNormal
    }
}
