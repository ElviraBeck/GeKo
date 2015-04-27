#version 330 core

uniform int useTexture;
uniform sampler2D tex;

in vec4 passPosition;
in vec3 passNormal;
in vec2 passUV;

layout(location = 0) out vec4 positionOutput;
layout(location = 1) out vec4 normalOutput;
layout(location = 2) out vec4 colorOutput;
 
void main(){  
	positionOutput = passPosition;
	normalOutput = vec4(normalize(passNormal), 1);
	colorOutput = vec4(0.0,1.0,1.0, 1.0f);

	if (useTexture != 0)
	{
		colorOutput = vec4(texture(tex, passUV).rgb, 1.0f);
	}
}