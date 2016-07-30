uniform sampler2D t_texture0;
varying vec2 v_texcoord0;
varying vec4 v_color;
varying vec3 v_normal;

void main(void)
{
	//gl_FragColor = texture2D(tex,vUv) * vec4(vNormal,1);
	//gl_FragColor = vec4(s_normal,1);

	gl_FragColor = v_color;
	//gl_FragColor = vec4(0,1,0,0);

	//vec4 colorT = texture2D(t_texture0, v_texcoord0);
	//vec4 u_color = vec4(1,1,1,1);
    //gl_FragColor = colorT * v_color;
	
}
	