#version 300 es
//设置 vPosition 的位置属性为 0
layout (location=0) in vec4 vPosition;
void main() {
  gl_Position = vPosition;
}