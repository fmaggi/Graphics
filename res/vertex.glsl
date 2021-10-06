#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;

out vec3 vColor;

uniform mat4 projview;

void main()
{
    vColor = color;
    gl_Position = projview * vec4(aPos, 1.0);
}
