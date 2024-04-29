#version 330 core
layout(lines) in;
layout(line_strip, max_vertices = 2) out;

void main()
{
    float lineWidth = 1.0;

    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;

    vec3 lineDirection = normalize(p1.xyz - p0.xyz);
    vec3 normal = normalize(cross(lineDirection, vec3(0, 0, 1)));
    vec3 offset = normal * lineWidth / 2.0;

    gl_Position = p0 + vec4(offset, 0.0);
    EmitVertex();

    gl_Position = p1 + vec4(offset, 0.0);
    EmitVertex();

    EndPrimitive();
}