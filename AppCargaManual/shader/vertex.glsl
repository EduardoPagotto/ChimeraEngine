attribute vec3 vertex;
attribute vec4 color;
attribute vec2 uv1;
attribute vec3 normal;

uniform vec3 lightPos; 
uniform vec4 lightColor;

varying vec2 v_texcoord0;
varying vec4 v_color;
varying vec4 v_lightColor;
varying vec3 v_normal;
varying vec3 v_FragPos;
varying vec3 v_lightPos;

uniform mat4 umvMat;
uniform mat4 upMat;
uniform mat4 modelMat;
uniform mat3 noMat;

uniform vec3 in_viewPos;
varying vec3 viewPos;

void main () {

	gl_Position = upMat * umvMat * modelMat * vec4(vertex, 1.0f);
	v_FragPos = vec3(modelMat * vec4(vertex, 1.0f));
	v_color = color;
	v_lightColor = lightColor;
	v_normal = mat3(transpose(inverse(modelMat))) * normal; //mudar para CPU
	v_lightPos = lightPos;
	viewPos = in_viewPos;

	//v_texcoord0 = uv1;
}
