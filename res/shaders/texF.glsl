#version 330 core

in vec2 v_uv;
in vec3 vColor;
uniform sampler2D u_texture;

out vec4 FragColor;

void main()
{
    FragColor = texture(u_texture, v_uv);
};