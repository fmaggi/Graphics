#version 450

layout (location = 0) in vec2 a_Pos;

void main() {

    gl_Position = vec4(a_Pos, 0, 1);

}
