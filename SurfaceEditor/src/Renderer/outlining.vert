/*
#version 330 core
layout (location = 0) in vec3 l_Pos;
layout (location = 1) in vec3 l_Normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform float outlining;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(l_Pos + l_Normal * outlining, 1.0f);
}
*/

#version 330 core
layout (location = 0) in vec3 l_Pos;
layout (location = 1) in vec3 l_Normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform float outlining; // This uniform now represents screen space outline size

void main()
{
    // Transform vertex position to clip space
    vec4 clipPos = u_projection * u_view * u_model * vec4(l_Pos, 1.0f);
    
    // Calculate screen space outline size
    //float screenSizeOutlineSize = outlining / clipPos.w;
    
    // Add outline size to vertex position
    vec3 offsetPos = l_Pos + l_Normal * outlining;
    
    // Transform vertex position to clip space
    clipPos = u_projection * u_view * u_model * vec4(offsetPos, 1.0f);
    
    gl_Position = clipPos;
}
