#version 330 core

// set: CanvasHMD.frag and CanvasHMD.vert
// used: Class CanvasHMD
// Render to texture to use in HMD

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main() {
    gl_Position = vec4(vertexPosition_modelspace, 1);
    UV = (vertexPosition_modelspace.xy + vec2(1, 1)) / 2.0;
}
