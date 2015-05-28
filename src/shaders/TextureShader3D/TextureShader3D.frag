#version 330 core

uniform sampler2D testTexture;

in vec3 passNormal;
in vec2 passUV;
out vec4 fragmentColor;

void main(){
	fragmentColor = texture(testTexture, passUV);
}