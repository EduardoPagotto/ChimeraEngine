#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec2 fragUV;

// Set 0: Dados globais da câmera (View e Projection)
layout(set = 0, binding = 0) uniform CameraBuffer {
    mat4 view;
    mat4 proj;
} camera;

// Push Constants: Dados que mudam por draw call
layout(push_constant) uniform PushConstants {
    mat4 model;
    int textureIndex;
} pc;

void main() {
    fragUV = inUV;
    gl_Position = camera.proj * camera.view * pc.model * vec4(inPosition, 1.0);
}
