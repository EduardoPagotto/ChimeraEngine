#version 330 core
//#version 300 es
precision mediump float;

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    sampler2D tDiffuse;
    sampler2D tSpecular;
    sampler2D tEmission;
    float shininess;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform int tipo;

void main() {

    vec4 ambient = material.ambient;
    vec4 diffuse = material.diffuse;
    vec4 specular = material.specular;

    if (tipo == 0) { // Sem Textura

        FragColor = (ambient + (diffuse + specular));

    } else if (tipo == 1) { // Apenas Diffuse

        vec4 color = vec4(texture(material.tDiffuse, TexCoords));
        FragColor = (ambient + (diffuse + specular)) * color;

    } else if (tipo == 2) { // Difusse e Specula
        // TODO
    } else {
        // TODO
    }

    // Core e Textura
    // FragColor = vec4(texture(material.tDiffuse, TexCoords)) * vec4(ourColor, 1.0);

    // So cor
    // FragColor = vec4(ourColor, 1.0);

    // So textura
    // FragColor = vec4(texture(material.tDiffuse, TexCoords));
}
