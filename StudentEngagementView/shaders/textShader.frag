#version 460 core

out vec4 fragColor;

in vec2 fragUV;

uniform isampler2D texTarget;
uniform vec4 color;

void main() {
  //fragColor = vec4(fragUV, 0, 1);
  fragColor = vec4(texture(texTarget, fragUV).x * color);
}