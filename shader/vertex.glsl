#version 300 es
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

//out vec2 TexCoords;

//out VS_OUT {
out    vec3 FragPos;
out    vec3 Normal;
out    vec2 TexCoords;
out    vec4 FragPosLightSpace;
//} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    //vs_out.
    FragPos = vec3(model * vec4(position, 1.0));
    //vs_out.
    Normal = transpose(inverse(mat3(model))) * normal;
    //vs_out.
    TexCoords = texCoords;
    //vs_out.
    //FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}
