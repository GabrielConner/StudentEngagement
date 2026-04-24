#version 460 core

layout(location=0) in vec2 pos;

uniform mat4 model;

out vec2 fragUV;

void main() {
  gl_Position = model * vec4(pos, 0, 1);
  fragUV = vec2(pos.x, 1 - pos.y);
}