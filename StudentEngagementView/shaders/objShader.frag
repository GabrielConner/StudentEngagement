#version 460 core

out vec4 fragColor;

in vec2 fragUV;

uniform vec4 color;
uniform sampler2D texTarget;

uniform bool useTex;

void main() {
  fragColor = useTex ? vec4(texture(texTarget, fragUV).xyz, 1) : color;
}