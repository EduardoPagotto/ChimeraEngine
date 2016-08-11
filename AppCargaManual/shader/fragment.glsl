uniform sampler2D t_texture0;
varying vec2 v_texcoord0;
varying vec4 v_color;
varying vec4 v_lightColor;
varying vec3 v_lightPos;
varying vec3 v_FragPos;

varying vec3 v_normal;

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
	vec3 result = (ambient + diffuse) * v_color;
	gl_FragColor = vec4(result, 1.0f);


   // vec3 result = ambient * v_color;
   //
   // gl_FragColor = vec4(result, 1.0f);


	//gl_FragColor = v_color;
	//gl_FragColor =  vec4(0.5f ,0.5f ,0.5f ,1.0f);
	//vec4 colorT = texture2D(t_texture0, v_texcoord0);
	//gl_FragColor = colorT * v_color;
	
}
	