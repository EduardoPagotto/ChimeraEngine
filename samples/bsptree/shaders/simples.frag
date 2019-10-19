#version 330 core
//#version 300 es
precision mediump float;

struct Material {
    sampler2D tDiffuse;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
// in vec3 ourColor;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;

void main() {
    // Core e Textura
    // FragColor = vec4(texture(material.tDiffuse, TexCoords)) * vec4(ourColor, 1.0);

    // So cor
    // FragColor = vec4(ourColor, 1.0);

    // So textura
    FragColor = vec4(texture(material.tDiffuse, TexCoords));
}
