#ifdef GL_ES
precision highp float;
#endif
//varying vec2 vUv;
//varying vec3 vNormal;

//uniform sampler2D tex;

//out vec3 color;

void main(void)
{
	//gl_FragColor = texture2D(tex,vUv) * vec4(vNormal,1);
	//gl_FragColor = vec4(s_normal,1);
	gl_FragColor = vec4(0,1,0,0);
	//color = vec3(1,0,0);
}
	