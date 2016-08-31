#version 300 es
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

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform int tipo;

void main()
{
  	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;

    // part Diffuse 
    vec3 normal = normalize(fs_in.Normal);
    //vec3 lightDir = normalize(-light.position); //direction light
	vec3 lightDir = normalize(light.position - fs_in.FragPos); //point light
    float diff = max(dot(normal, lightDir), 0.0);

    // part Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	if (tipo == 0) { //Sem Textura

		ambient = light.ambient * material.ambient;
		diffuse = light.diffuse * (diff * material.diffuse);
		specular = light.specular * (spec * material.specular);
		FragColor = ambient + diffuse + specular;

	} else if (tipo == 1) { //Apenas Diffuse

		vec4 color = vec4(texture(material.tDiffuse, fs_in.TexCoords));

		ambient = light.ambient * color;
		diffuse = light.diffuse * diff * color;	
		specular = light.specular * (spec * material.specular);

		FragColor = ambient + diffuse + specular;
	
	} else if (tipo == 2) { //Difusse e Specula

		vec4 color = vec4(texture(material.tDiffuse, fs_in.TexCoords));

		ambient = light.ambient * color;
		diffuse = light.diffuse * diff * color;
		specular = light.specular * spec * vec4(texture(material.tSpecular, fs_in.TexCoords));

		FragColor = ambient + diffuse + specular + emission;
	
	} else { 

		vec4 color = vec4(texture(material.tDiffuse, fs_in.TexCoords));

		ambient = light.ambient * color;
		diffuse = light.diffuse * diff * color;
		specular = light.specular * spec * vec4(texture(material.tSpecular, fs_in.TexCoords));
	
		// Emission
		emission = vec4(texture(material.tEmission, fs_in.TexCoords));
		FragColor = ambient + diffuse + specular + emission;	
	}
} 
	