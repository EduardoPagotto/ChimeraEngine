#version 460
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

// Set 1: Estrutura Bindless
layout(set = 1, binding = 0) uniform sampler globalSampler;          // Um sampler estático
layout(set = 1, binding = 1) uniform texture2D textures[];           // Array infinito de texturas

layout(push_constant) uniform PushConstants {
    mat4 model;
    int textureIndex; // Índice recebido da CPU via Push Constant
} pc;

void main() {
    // nonuniformEXT garante que diferentes instâncias/pixels possam acessar índices distintos simultaneamente
    int index = nonuniformEXT(pc.textureIndex);

    // Combina a textura dinâmica indexada com o sampler global
    outColor = texture(sampler2D(textures[index], globalSampler), fragUV);
}
