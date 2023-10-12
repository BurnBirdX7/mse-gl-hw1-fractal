#version 330 core

out vec4 frag_color;
in vec2 tex_coord;

uniform float border_value;
uniform int max_iterations;
uniform float scale;
uniform vec2 center;
uniform float aspect;

vec2 pow2(vec2 c) {
    float x = pow(c.x, 2) - pow(c.y, 2);
    float y = 2 * c.x * c.y;
    return vec2(x, y);
}

vec3 iter(vec2 coord) {
    vec2 z = vec2(0, 0);
    int i;
    float avg_dist = 0.0f;

    for (i = 1; i < max_iterations; i++) {
        z = pow2(z) + coord;
        float len = length(z);
        if (len > border_value) {
            break;
        }

        // track stats for coloring
        avg_dist = avg_dist + len * len;
    }

    float v = i / max_iterations;
    avg_dist /= i;
    return vec3(avg_dist + 0.15 * v, 0.3 * v, v);

}

void main() {
    vec2 c;
    c.x = aspect * tex_coord.x;
    c.y = tex_coord.y;
    c = c * scale - center;
    vec3 v = iter(c);
    frag_color = vec4(v, 1.0);
}
