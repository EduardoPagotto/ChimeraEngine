#version 450

// Vértices hardcoded para um retângulo (Full-Screen Quad)
vec2 positions[4] = vec2[](
    vec2(-1.0, -1.0), // Inferior Esquerdo
    vec2( 1.0, -1.0), // Inferior Direito
    vec2(-1.0,  1.0), // Superior Esquerdo
    vec2( 1.0,  1.0)  // Superior Direito
);

vec2 uvs[4] = vec2[](
    vec2(0.0, 1.0), // Inverte o eixo Y padrão das superfícies do SDL
    vec2(1.0, 1.0),
    vec2(0.0, 0.0),
    vec2(1.0, 0.0)
);

layout(location = 0) out vec2 fragTexCoord;

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragTexCoord = uvs[gl_VertexIndex];
}
