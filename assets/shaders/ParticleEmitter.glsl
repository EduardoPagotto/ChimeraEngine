#type vertex
#version 440 core

// set: ParticleEmitter.frag and ParticleEmitter.vert
// used: Class ParticleEmitter to app models
// Render Particle emitter

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs;  // Position of the center of the particule and size of the square
layout(location = 2) in vec4 color; // Position of the center of the particule and size of the square

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec4 particlecolor;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
// uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the
// orientation depends on the camera)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    float particleSize = xyzs.w; // because we encoded it this way.
    vec3 particleCenter_wordspace = xyzs.xyz;

    vec3 vertexPosition_worldspace = particleCenter_wordspace + CameraRight_worldspace * squareVertices.x * particleSize +
                                     CameraUp_worldspace * squareVertices.y * particleSize;

    // Output position of the vertex
    gl_Position = projection * view * model * vec4(vertexPosition_worldspace, 1.0f);

    // UV of the vertex. No special space for this one.
    UV = squareVertices.xy + vec2(0.5, 0.5);
    particlecolor = color;
}
//---
#type fragment
#version 440 core

// set: ParticleEmitter.frag and ParticleEmitter.vert
// used: Class ParticleEmitter to app models
// Render Particle emitter

precision mediump float;

#include MaterialDef.glsl

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