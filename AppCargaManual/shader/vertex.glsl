attribute vec3 vertex;
attribute vec4 color;
attribute vec2 uv1;
attribute vec3 normal;

//out vec4 eyeCordFs;
//out vec4 eyeNormalFs;

varying vec2 v_texcoord0;
varying vec4 v_color;
varying vec3 v_normal;

uniform mat4 umvMat;
uniform mat4 upMat;
uniform mat3 noMat;

void main () {

        gl_Position = upMat * umvMat * vec4(vertex, 1.0);

		//vec4 eyeNorm = normalize(noMat * vec4(normal, 0.0));
		//vec4 eyeCord= modelView * vec4(vertex, 1.0);

		//eyeCordFs = eyeCord;
		//eyeNormalFs = eyeNorm;
		
		v_color = color;
		v_normal = normal;
		v_texcoord0 = uv1;
}
