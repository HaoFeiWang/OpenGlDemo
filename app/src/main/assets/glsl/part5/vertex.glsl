#version 300 es

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec2 in_texture;

out vec2 frag_texture_position;

void main() {
    gl_Position = in_position;
    frag_texture_position = in_texture;
}
