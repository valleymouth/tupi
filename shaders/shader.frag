#version 450

layout(location = 0) in vec3 color;
layout(location = 1) in vec2 texCoord;
layout(binding = 1) uniform sampler2D texSampler;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(texture(texSampler, texCoord).rgb * color, 1.0);
}