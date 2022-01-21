#type vertex
#version 440 core
// set: MeshNoMat.frag and MeshNoMat.vert
// Render Mesh with texture and without material set

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out DATA_FRAG {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
}
vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vs_out.FragPos = vec3(model * vec4(position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * normal;
    vs_out.TexCoords = texCoords;
}

//---

#type fragment
#version 440 core
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
