#version 300 es
precision mediump float;

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
}; 
  
uniform Material material;

out vec4 color;
in vec3 Normal; 
in vec3 FragPos;  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec4 lightColor;

void main()
{
    // Ambient
    vec4 ambient = lightColor * material.ambient;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = lightColor * (diff * material.diffuse);
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = lightColor * (spec * material.specular);  
        
    vec4 result = ambient + diffuse + specular;
    color = result;//vec4(result, 1.0f);
} 
	