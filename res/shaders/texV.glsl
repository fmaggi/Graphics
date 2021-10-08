#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv_Coords;
layout (location = 3) in float texIndex;

out vec3 vColor;
out vec2 v_uv;
out float v_texIndex;

uniform mat4 projview;

void main()
{
    v_texIndex = texIndex;
    vColor = color;
    v_uv = uv_Coords;
    gl_Position = projview * vec4(aPos, 1.0);
}