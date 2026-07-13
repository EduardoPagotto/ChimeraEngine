#version 450

layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

// No SDL3 GPU, texturas/samplers costumam ocupar o set espacial 2 por padrão
layout(set = 2, binding = 0) uniform sampler2D surfaceTexture;

void main() {
    outColor = texture(surfaceTexture, fragTexCoord);
}
