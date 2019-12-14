#version 330 core
//#version 300 es

// set: MeshNoMat.frag and MeshNoMat.vert
// used: app bsptree
// Render Mesh with texture and without material set

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
    // color and texture
    // FragColor = vec4(texture(material.tDiffuse, TexCoords)) * vec4(ourColor, 1.0);

    // only color
    // FragColor = vec4(ourColor, 1.0);

    // only texture
    FragColor = vec4(texture(material.tDiffuse, TexCoords));
}
