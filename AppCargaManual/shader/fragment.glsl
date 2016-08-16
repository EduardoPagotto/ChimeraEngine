#version 300 es
precision mediump float;

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    sampler2D tDiffuse;
    sampler2D tSpecular;
    float shininess;
}; 
  
struct Light {
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

out vec4 color;

in vec3 Normal; 
in vec3 FragPos;  
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform int tipo;

void main()
{
  	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	if (tipo == 0) { //Sem Textura

		ambient = light.ambient * material.ambient;
		diffuse = light.diffuse * (diff * material.diffuse);
		specular = light.specular * (spec * material.specular);

	} else if (tipo == 1) { //Apenas Diffuse

		ambient = light.ambient * vec4(texture(material.tDiffuse, TexCoords));
		diffuse = light.diffuse * diff * vec4(texture(material.tDiffuse, TexCoords));	
		specular = light.specular * (spec * material.specular);
	
	} else if (tipo == 2) { //Difusse e Specula

		ambient = light.ambient * vec4(texture(material.tDiffuse, TexCoords));
		diffuse = light.diffuse * diff * vec4(texture(material.tDiffuse, TexCoords));
		specular = light.specular * spec * vec4(texture(material.tSpecular, TexCoords));
	
	}

    color = ambient + diffuse + specular;
} 
	