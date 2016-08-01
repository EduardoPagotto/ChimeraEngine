attribute vec3 vertex;
attribute vec4 color;
attribute vec2 uv1;
attribute vec3 normal;

varying vec2 v_texcoord0;
varying vec4 v_color;
varying vec3 v_normal;

uniform mat4 umvMat;
uniform mat4 upMat;

void main () {

        gl_Position = upMat * umvMat * vec4(vertex, 1.0);

		v_color = color;
		v_normal = normal;
		v_texcoord0 = uv1;
}
