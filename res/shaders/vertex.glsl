#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float rotation;
layout (location = 2) in vec2 scale;
layout (location = 3) in vec2 base;
layout (location = 4) in vec4 color;
layout (location = 5) in vec2 uv_Coords;
layout (location = 6) in float texIndex;

out vec4 vColor;
out vec2 v_uv;

uniform mat4 projview;

void main()
{
    vColor = color;
    v_uv = uv_Coords;

    mat2 scaleMat = mat2(scale.x, 0, 0, scale.y);
    mat2 rotMat = mat2(cos(rotation), sin(rotation), -sin(rotation), cos(rotation));
    vec2 pos = aPos.xy + (scaleMat * rotMat * base);

    gl_Position = projview * vec4(pos, aPos.z, 1.0);
}
