#version 300 es

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texture;

out vec2 frag_texture_position;

uniform mat4 rotate;
uniform mat4 transform;
uniform mat4 perspective;

void main() {
    gl_Position = perspective * transform * rotate * vec4(in_position, 1.0f);
    frag_texture_position = in_texture;
}
