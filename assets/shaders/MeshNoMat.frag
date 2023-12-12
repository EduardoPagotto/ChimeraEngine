#version 430 core
// set: MeshNoMat.frag and MeshNoMat.vert
// Render Mesh with texture and without material set

precision mediump float;

layout(location = 0) out vec4 FragColor; // framebuffer color (0)
layout(location = 1) out int color2;     // framebuffer custom (1) (mouse pick)

struct Material {
    sampler2D tDiffuse;
};

in DATA_FRAG {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
}
vs_in;

uniform vec3 viewPos;
uniform Material material;

void main() {
    // color and texture
    // FragColor = vec4(texture(material.tDiffuse, TexCoords)) * vec4(ourColor, 1.0);
    FragColor = vec4(texture(material.tDiffuse, vs_in.TexCoords));
    color2 = 50;
}
