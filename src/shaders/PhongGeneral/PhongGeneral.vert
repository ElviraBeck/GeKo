#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 passPosition;
out vec3 passNormal;

void main(){
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
	passPosition = viewMatrix * modelMatrix * position;

	mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
	passNormal = normalize(normalMatrix * normal);
}