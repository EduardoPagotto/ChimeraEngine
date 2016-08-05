attribute vec3 vertex;
attribute vec4 color;
attribute vec2 uv1;
attribute vec3 normal;

uniform vec3 l_dir; 

varying vec2 v_texcoord0;
varying vec4 v_color;
//varying vec3 v_normal;

uniform mat4 umvMat;
uniform mat4 upMat;
uniform mat3 noMat;

void main () {

	gl_Position = upMat * umvMat * vec4(vertex, 1.0);

	vec3 n = normalize(noMat * normal);//http://www.lighthouse3d.com/tutorials/glsl-tutorial/directional-lights/
	
	// compute the intensity as the dot product
	// the max prevents negative intensity values
	float intensity = max(dot(n, l_dir), 0.0);
	
	v_color = intensity * color;
	
	//v_color = color;
	//v_normal = normal;
	v_texcoord0 = uv1;
}
