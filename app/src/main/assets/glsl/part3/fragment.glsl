#version 300 es

precision mediump float;

in vec3 frag_color;
out vec4 out_frag_color;

void main() {
    out_frag_color = vec4(frag_color, 1.0);
}
