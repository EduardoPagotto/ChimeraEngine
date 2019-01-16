#version 330 core
//#version 300 es
precision mediump float;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 ourColor;

out vec4 FragColor;

uniform vec3 viewPos;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
