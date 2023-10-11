#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 corner;

out vec2 tex_coord;

void main() {
    tex_coord = corner;
    gl_Position = vec4(pos, 0.0, 1.0);
}