#type vertex
#version 300 es

// set: debug_quad.frag and debug_quad.vert
// used: desconhecido
// ???

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(position, 1.0f);
    TexCoords = texCoords;
}
//---
#type fragment
#version 300 es

// set: debug_quad.frag and debug_quad.vert
// used: desconhecido
// ???

precision mediump float;

out vec4 color;
in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main() {
    float depthValue = texture(depthMap, TexCoords).r;
    // color = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    color = vec4(vec3(depthValue), 1.0); // orthographic
}