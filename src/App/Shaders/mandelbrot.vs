#version 330 core

layout (location = 0) in vec2 pos;

out vec2 tex_coord;

void main() {
    tex_coord = pos;
    gl_Position = vec4(pos, 0.0, 1.0);
}