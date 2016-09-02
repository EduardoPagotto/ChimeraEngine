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

uniform sampler2D shadowMap;

uniform bool shadows;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform int tipo;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
            
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
    // part Diffuse 
    vec3 normal = normalize(fs_in.Normal);
    //vec3 lightDir = normalize(-light.position); //direction light
	vec3 lightDir = normalize(light.position - fs_in.FragPos); //point light
    float diff = max(dot(normal, lightDir), 0.0);

    // part Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec4 ambient = light.ambient * material.ambient;
	vec4 diffuse = light.diffuse * (diff * material.diffuse);
	vec4 specular = light.specular * (spec * material.specular);

	float shadow = shadows ? ShadowCalculation(fs_in.FragPosLightSpace) : 0.0;

	if (tipo == 0) { //Sem Textura

		FragColor = (ambient + (1.0 - shadow) * (diffuse + specular)); 

	} else if (tipo == 1) { //Apenas Diffuse

		vec4 color = vec4(texture(material.tDiffuse, fs_in.TexCoords));
		FragColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * color; 
	
	} else if (tipo == 2) { //Difusse e Specula
		//TODO
	} else { 
		//TODO
	}
} 
	