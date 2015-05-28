#version 430 core

in vec4 pos;
out vec4 fragmentColor;

void main(){
	fragmentColor = vec4(pos.xyz, 1);
}