#version 300 es

precision mediump float;

in vec2 frag_texture_position;
out vec4 out_frag_color;

uniform sampler2D texture_sample1;
uniform sampler2D texture_sample2;

void main() {
    out_frag_color = mix(
        texture(texture_sample1, frag_texture_position),
        texture(texture_sample2, frag_texture_position),
        0.2
    );
}
