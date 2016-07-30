attribute vec3 vertex;
attribute vec4 color;
attribute vec2 uv1;
attribute vec3 normal;

varying vec2 v_texcoord0;
varying vec4 v_color;
varying vec3 v_normal;

//attribute vec3 aColor;
uniform mat4 umvMat;
uniform mat4 upMat;

void main () {

        gl_Position = upMat * umvMat * vec4(vertex, 1.0);

		v_color = color;
		v_normal = normal;
		v_texcoord0 = uv1;
}

// attribute vec3 vertex;
// attribute vec3 normal;
// attribute vec2 uv1;
// 
// uniform mat4 _mvProj;
// uniform mat3 _norm;
// 
// //varying vec2 vUv;
// //varying vec3 vNormal;
// 
// void main(void) {
//  // compute position
//  gl_Position = _mvProj * vec4(vertex, 1.0);
//  
// 
//  //gl_Position.xyz = vertex;
//  //gl_Position.w = 1.0;
// 
//  //vUv = uv1;
//  // compute light info
//  //vNormal= _norm * normal;
// 
// } 