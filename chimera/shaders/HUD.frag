#version 300 es

// set: HUD.frag and HUD.vert
// used: Class HUD
// Render HUD to texture

precision mediump float;

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = textColor * sampled;
}
