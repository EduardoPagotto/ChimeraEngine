#version 450

layout(location = 0) in vec3 fragCol; // Interpolated colour from vertex (location must match)
layout(location = 1) in vec2 fragTex;

layout(set = 1, binding = 0) uniform sampler2D textureSampler;

layout(location = 0) out vec4 outColour; // Final output colour (must also have location)

void main() {
    //outColour = vec4(fragCol, 1.0); // red
    outColour = texture(textureSampler, fragTex);
}
