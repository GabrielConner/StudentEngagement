#version 460 core

layout(location=0) in vec2 pos;
layout(location=1) in vec2 uv;

uniform mat4 model;

out vec2 fragUV;

void main() {
  gl_Position = model * vec4(pos, 0, 1);
  fragUV = uv;
}