#version 330 core

out vec4 frag_color;
in vec2 tex_coord;

uniform int border_value;
uniform int max_iterations;
uniform float scale;
uniform vec2 center;
uniform float aspect;

vec2 pow2(vec2 c) {
    float x = pow(c.x, 2) - pow(c.y, 2);
    float y = 2 * c.x * c.y;
    return vec2(x, y);
}

float iter(vec2 coord) {
    vec2 z = vec2(0, 0);
    for (int i = 0; i < max_iterations; i++) {
        z = pow2(z) + coord;
        if (length(z) > border_value) {
            return i / max_iterations;
        }
    }
    return max_iterations;
}

void main() {
    vec2 c;
    c.x = aspect * tex_coord.x;
    c.y = tex_coord.y;
    c = c * scale - center;
    float v = iter(c);
    frag_color = vec4(v, v, v, 1.0);
}
