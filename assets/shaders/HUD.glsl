#type vertex
#version 300 es

// set: HUD.frag and HUD.vert
// used: Class HUD
// Render HUD to texture

layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
//----
#type fragment
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