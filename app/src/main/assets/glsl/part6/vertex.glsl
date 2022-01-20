#version 300 es

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texture;

uniform mat4 transform;
out vec2 frag_texture_position;

void main() {
    gl_Position = transform * vec4(in_position, 1.0f);
    frag_texture_position = in_texture;
}
