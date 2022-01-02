#version 450 core

in vec2 v_uv;
in vec4 vColor;
in float v_texIndex;

layout (location = 0) uniform sampler2D u_texture[16];

out vec4 FragColor;

void main()
{
    vec4 texColor;
    switch (int(v_texIndex))
    {
        case -1: texColor = vec4(1, 1, 1, 1); break;
        case  0: texColor = texture(u_texture[0], v_uv); break;
        case  1: texColor = texture(u_texture[1], v_uv); break;
    }

    if (texColor.w < 0.1f)
        discard;
    FragColor = texColor * vColor;
};
