#version 430 core

layout (location = 0) in vec4 position;	

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out float lifetimeparticle;

void main(){
	lifetimeparticle = position.w;
    gl_Position = position;
}