#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;

out vec3 o_color;

void main()
{
    o_color = (aPos + 1) * 0.5;
    gl_Position = vec4(aPos, 1.0);
}
