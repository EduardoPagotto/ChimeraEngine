#version 130 //#version 300 es
uniform sampler2D t_texture0;
varying vec2 v_texcoord0;
varying vec4 v_color;
varying vec4 v_lightColor;
varying vec3 v_lightPos;
varying vec3 v_FragPos;

varying vec3 v_normal;

varying vec3 viewPos;

void main(void)
{
	//Ambient
	float ambientStrength = 0.05f;
    vec4 ambient = ambientStrength * v_lightColor;

	//Diffuse
	vec3 norm = normalize(v_normal);
	vec3 lightDir = normalize(v_lightPos - v_FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * v_lightColor;

	//Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //2 a 256 altera o tamanho 
    vec4 specular = specularStrength * spec * v_lightColor;

	vec4 result = (ambient + diffuse + specular) * v_color;
    gl_FragColor = result;//vec4(result, 1.0f);

	//vec4 colorT = texture2D(t_texture0, v_texcoord0);
	//gl_FragColor = colorT * v_color;
	
}
	