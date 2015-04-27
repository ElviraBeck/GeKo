#version 330

in vec2 passUV;

uniform sampler2D colortexture;
uniform float yAxis;

layout(location = 2) out vec4 colorOutput;

void main() {
	float weights[4];
	weights[0] = 2.0f;
	weights[1] = 3.0f;
	weights[2] = 2.0f;
	weights[3] = 1.0f;
    
	vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
	vec2 diff = vec2(1.0, 0.0);
	if (yAxis > 0.5) diff = vec2(0.0, 1.0);
	diff *= (1.0 / 800.0);
	
	for(int i=0; i<4; i++)
	{
		sum += 2.0 * texture(colortexture, passUV + i*diff);
		sum += 2.0 * texture(colortexture, passUV - i*diff);
	}
	
	colorOutput = sum / 16.0;
}