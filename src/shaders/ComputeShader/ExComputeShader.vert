#version 430 core

in vec4 vert;
out vec4 pos;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	pos = vert;
    gl_Position = projectionMatrix * viewMatrix * vec4(vert.xyz, 1.0);
}