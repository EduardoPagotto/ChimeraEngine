#version 120

uniform mat4x4 ShadowMatrix;

varying vec3 var_Normal;

void main()
{
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = ShadowMatrix * gl_Vertex;
	var_Normal = gl_Normal;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

