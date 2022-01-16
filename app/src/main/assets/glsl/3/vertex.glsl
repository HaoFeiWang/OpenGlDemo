#version 300 es

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec3 in_color;

out vec3 frag_color;

void main() {
    frag_color = in_color;
    gl_Position = in_position;
}
