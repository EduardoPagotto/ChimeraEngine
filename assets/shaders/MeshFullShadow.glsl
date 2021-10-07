#type vertex
#version 330 core
//#version 300 es

// set: MeshFullShadow.frag and MeshFullShadow.vert
// used: Class RenderVisitos
// Render Texture, material shadows

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// out vec2 TexCoords;

// out VS_OUT {
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;
//} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    // vs_out.
    FragPos = vec3(model * vec4(position, 1.0));
    // vs_out.
    Normal = transpose(inverse(mat3(model))) * normal;
    // vs_out.
    TexCoords = texCoords;
    // vs_out.
    // FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}

//---

#type fragment
#version 330 core
//#version 300 es

// set: MeshFullShadow.frag and MeshFullShadow.vert
// used: Class RenderVisitos
// Render Texture, material shadows

precision mediump float;

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    sampler2D tDiffuse;
    sampler2D tSpecular;
    sampler2D tEmission;
    float shininess;
};

struct Light {
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

// in VS_OUT {
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;
//} fs_in;

out vec4 FragColor;

uniform sampler2D shadowMap;

uniform bool shadows;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform int tipo;

float ShadowCalculation(vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    // vec3 lightDir = normalize(light.position - FragPos); //light point
    vec3 lightDir = normalize(light.position); // light dir (invertido do exemplo)

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    // vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main() {
    // part Diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position); // direction light
                                               // vec3 lightDir = normalize(light.position - FragPos); //point light
    float diff = max(dot(normal, lightDir), 0.0);

    // part Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec4 ambient = light.ambient * material.ambient;
    vec4 diffuse = light.diffuse * (diff * material.diffuse);
    vec4 specular = light.specular * (spec * material.specular);

    float shadow = shadows ? ShadowCalculation(FragPosLightSpace) : 0.0;

    if (tipo == 0) { // Sem Textura

        FragColor = (ambient + (1.0 - shadow) * (diffuse + specular));

    } else if (tipo == 1) { // Apenas Diffuse

        vec4 color = vec4(texture(material.tDiffuse, TexCoords));
        FragColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    } else if (tipo == 2) { // Difusse e Specula
        // TODO
    } else {
        // TODO
    }
}
