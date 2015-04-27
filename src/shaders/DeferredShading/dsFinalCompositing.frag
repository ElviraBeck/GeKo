#version 330

in vec2 passUVCoord;

uniform sampler2D colorMap;
uniform sampler2D lightMap;

layout(location = 2) out vec4 colorOutput;

void main() {
    vec4 light = texture(lightMap, passUVCoord);
    vec4 color = texture(colorMap, passUVCoord);

    float resX = 1.0/400.0;
    float resY = 1.0/400.0;

    int strength = 5;

    float glow = 0;
    for(int i = -strength; i < strength; i++) {
        for(int j = -strength; j < strength; j++) {
            vec4 c = texture(lightMap, passUVCoord + vec2(resX * i, resY * j));       
            glow += (c.r + c.g + c.b)/3.0;
        }
    }
    glow /= strength * strength * 4;
	glow *= glow;

	colorOutput = color * light;
	if(color.a < 0.5) 
		colorOutput = vec4(0.0,0.0,0.0,0.0);

	colorOutput *= (0.3 + glow);
}