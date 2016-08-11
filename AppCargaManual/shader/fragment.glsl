#version 300 es
precision mediump float;
out vec4 color;
in vec3 Normal; 
in vec3 FragPos;  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec4 lightColor;
uniform vec4 objectColor;

// uniform sampler2D t_texture0;
// varying vec2 v_texcoord0;

void main()
{
    // Ambient
    float ambientStrength = 0.1f;
    vec4 ambient = ambientStrength * lightColor;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = specularStrength * spec * lightColor;  
        
    vec4 result = (ambient + diffuse + specular) * objectColor;
	color = result;
} 

	