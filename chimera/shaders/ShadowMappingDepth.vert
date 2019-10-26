#version 300 es

// set: ShadowMappingDepth.frag and ShadowMappingDepth.vert
// used: Class ShadowMapVisitor to app models
// Render Shadowmap

layout(location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() {
    // calc
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
}