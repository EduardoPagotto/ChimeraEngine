#version 330 core
// in vec3 fColor;

// in VS_DATA { vec3 color; }
// fs_in;
in vec3 fColorZ1;

out vec4 color;

// void main() { color = vec4(fs_in.color, 1.0f); }
void main() { color = vec4(fColorZ1, 1.0f); }