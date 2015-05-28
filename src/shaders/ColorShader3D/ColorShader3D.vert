#version 330 core

layout (location = 0) in vec4 positionAttribute;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(){
  gl_Position = projectionMatrix * viewMatrix* positionAttribute;
}