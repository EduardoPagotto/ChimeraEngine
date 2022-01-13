#type vertex
#version 440 core
// set: MeshNoMat.frag and MeshNoMat.vert
// used: app bsptree
// Render Mesh with texture and without material set

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = transpose(inverse(mat3(model))) * normal;
    TexCoords = texCoords;
}

//---

#type fragment
#version 330 core
//#version 300 es
// set: MeshNoMat.frag and MeshNoMat.vert
// used: app bsptree
// Render Mesh with texture and without material set

precision mediump float;

layout(location = 0) out vec4 FragColor; // framebuffer color (0)
layout(location = 1) out int color2;     // framebuffer custom (1) (mouse pick)

struct Material {
    sampler2D tDiffuse;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;

void main() {
    // color and texture
    // FragColor = vec4(texture(material.tDiffuse, TexCoords)) * vec4(ourColor, 1.0);

    FragColor = vec4(texture(material.tDiffuse, TexCoords));
    color2 = 50;
}
