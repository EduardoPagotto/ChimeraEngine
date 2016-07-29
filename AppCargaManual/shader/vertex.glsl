attribute vec3 vertex;
//attribute vec3 aColor;
uniform mat4 umvMat;
uniform mat4 upMat;
//varying vec3 vColor;
void main () {
        gl_Position = upMat * umvMat * vec4(vertex, 1.0);
        //vColor = aColor;
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