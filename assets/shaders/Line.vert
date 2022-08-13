#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

// out VS_DATA { vec3 color; }
// vs_out;
out vec3 fColorZ1;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * vec4(position, 1.0f);
    // vs_out.color = color;
    fColorZ1 = color;
}