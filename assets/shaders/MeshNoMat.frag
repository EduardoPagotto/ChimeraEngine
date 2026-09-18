#version 430 core
// set: MeshNoMat.frag and MeshNoMat.vert
// Render Mesh with texture and without material set

precision mediump float;

layout(location = 0) out vec4 FragColor; // framebuffer color (0)
//layout(location = 1) out int color2;     // framebuffer custom (1) (mouse pick)

struct Material {
    vec4 ambient; // not used
    vec4 diffuse; // not used
    vec4 specular; // not used
    vec4 emissive; // not used
    sampler2D tDiffuse;
    float shininess; // not used
};

in DATA_FRAG {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
}
vs_in;

uniform vec3 viewPos;
uniform Material material;
uniform int tipo; // FIXME: refazer depois, desnecessario

void main() {
    // color and texture
    //FragColor = vec4(texture(material.tDiffuse, vs_in.TexCoords));
    // color2 = 50 + tipo;

    if (tipo == 0) { // Sem Textura
        FragColor = (material.ambient + (material.diffuse + material.specular));
    } else if (tipo == 1) { // Apenas Diffuse
        FragColor = vec4(texture(material.tDiffuse, vs_in.TexCoords));
    }
}
