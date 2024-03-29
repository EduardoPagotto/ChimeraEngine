#version 430 core

// set: ParticleEmitter.frag and ParticleEmitter.vert
// used: Class ParticleEmitter to app models
// Render Particle emitter

precision mediump float;

// Material def
struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    sampler2D tDiffuse;
    sampler2D tSpecular;
    sampler2D tEmission;
    float shininess;
};

uniform Material material;
uniform int tipo;

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 particlecolor;

// Ouput data
out vec4 color;

uniform sampler2D myTextureSampler;

void main() {
    if (tipo == 1)
        color = texture(material.tDiffuse, UV) * particlecolor;
    else
        color = texture(material.tDiffuse, UV) * particlecolor;
}