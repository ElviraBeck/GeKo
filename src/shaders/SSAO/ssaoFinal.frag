#version 330

in vec2 passUV;

uniform sampler2D colorMap;
uniform sampler2D ssaoMap;

layout(location = 2) out vec4 colorOutput;

void main()
{
	colorOutput = texture(colorMap,passUV);
	colorOutput *= (texture(ssaoMap,passUV) * 0.7 + 0.3);
}