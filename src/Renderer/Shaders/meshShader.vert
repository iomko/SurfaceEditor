#version 330 core
layout (location = 0) in vec3 l_Pos;
layout (location = 1) in vec3 l_Normal;
layout (location = 2) in float l_IsHighlighted;
layout (location = 3) in float l_IsSkewed;

out vec3 v_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    mat4 mvp = u_projection * u_view * u_model;
    gl_Position = mvp * vec4(l_Pos, 1.0);

    if (l_IsHighlighted == 1.0f)
        v_color = vec3(255.0 / 255.0, 198.0 / 255.0, 111.0 / 255.0);
    else
        v_color = vec3(0.5, 0.5, 0.5);

    if (l_IsSkewed == 1.0f) {
        v_color = vec3(1.0, 0.0, 0.0);
    } else if (l_IsSkewed == 0.5f) {
        v_color = vec3(0.0, 1.0, 0.0);
    }
}
