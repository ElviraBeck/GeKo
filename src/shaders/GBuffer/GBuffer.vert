#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;  

uniform int useShadowMap;
uniform mat4 lightVPBias;

out vec4 passPosition;
out vec3 passNormal;
out vec2 passUV;

out vec3 passWorldNormal;
out mat3 normalMatrix;

out vec4 passShadowCoord;

out vec3 passSkyboxTexCoord;
 
void main(){

	mat4 MV = viewMatrix * modelMatrix;

    gl_Position = projectionMatrix * MV * position;

	passPosition = MV * position;
	normalMatrix = mat3(transpose(inverse(MV)));
	passNormal = normalize(normalMatrix * normal);
	passUV = uv;
	
	passWorldNormal = normal;

	passSkyboxTexCoord = position.xyz;

	if(useShadowMap != 0)
	{
		passShadowCoord = lightVPBias * modelMatrix * position;
	}
}
