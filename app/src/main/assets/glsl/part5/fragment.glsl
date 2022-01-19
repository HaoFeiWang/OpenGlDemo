#version 300 es

precision mediump float;

in vec2 frag_texture_position;
out vec4 out_frag_color;

uniform sampler2D texture_sample;

void main() {
    out_frag_color = texture(texture_sample, frag_texture_position);
}
