#version 330 core

layout (location = 0) in vec4 positionAttribute;


uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 passTexCoord;

void main(){
  passTexCoord = positionAttribute.xyz;

  mat4 vM = mat4(mat3(viewMatrix));

  gl_Position = projectionMatrix * vM * positionAttribute;
}