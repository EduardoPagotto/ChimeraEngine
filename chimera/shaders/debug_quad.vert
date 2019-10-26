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