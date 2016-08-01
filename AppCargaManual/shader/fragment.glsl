uniform sampler2D t_texture0;
varying vec2 v_texcoord0;
varying vec4 v_color;
varying vec3 v_normal;

void main(void)
{
	//vec4 colorT = texture2D(t_texture0, v_texcoord0) * vec4(v_normal,1);
	vec4 colorT = texture2D(t_texture0, v_texcoord0);
	gl_FragColor = colorT * v_color;
	
}
	