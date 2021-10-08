#version 330 core

in vec2 v_uv;
in vec3 vColor;
in float v_texIndex;

uniform sampler2D u_texture[16];

out vec4 FragColor;

void main()
{
    switch (int(v_texIndex))
    {
        case 0: FragColor = texture(u_texture[0], v_uv); break;
        case 1: FragColor = texture(u_texture[1], v_uv); break;
    }
};