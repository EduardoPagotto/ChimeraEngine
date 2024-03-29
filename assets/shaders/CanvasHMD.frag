#version 430 core

// set: CanvasHMD.frag and CanvasHMD.vert
// used: Class CanvasHMD
// Render to texture to use in HMD

in vec2 UV;
out vec3 color;

uniform sampler2D renderedTexture;
uniform float time;

void main() {
    // color = texture( renderedTexture, UV + 0.005*vec2( sin(time+1024.0*UV.x),cos(time+768.0*UV.y)) ).xyz ;
    // color = texture( renderedTexture, UV + 0.005*vec2( sin(time+512.0*UV.x),cos(time+512.0*UV.y)) ).xyz ;
    color = texture(renderedTexture, UV).xyz;
}