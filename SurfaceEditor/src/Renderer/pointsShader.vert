#version 330 core
layout (location = 0) in vec3 position;
layout(location = 1) in float isHighlighted;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 pointColor;


void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);


    if (isHighlighted == 1.0f) {
        pointColor = vec3(255.0f / 255.0f, 95.0f / 255.0f , 95.0f / 255.0f);
    } else {
        pointColor = vec3(54.0f / 255.0f, 54.0f / 255.0f, 54.0f / 255.0f);
    }
}
