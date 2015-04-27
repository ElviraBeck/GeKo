#version 330

in vec2 passUVCoord;

uniform sampler2D colorTexture;

uniform int fboWidth;
uniform int fboHeight;

layout(location = 2) out vec4 colorOutput;

void main()
{
	float FXAA_SPAN_MAX = 8.0;
	float FXAA_REDUCE_MIN = 1.0/128.0;
	float FXAA_REDUCE_MUL = 1.0/8.0;
	
	//get luminosity of surrounding pixels
	vec3 luma = vec3(0.299,0.587,0.114); //official values for getting luminosity value of a color
	
	vec2 texCoordOffset = vec2(1.0/fboWidth,1.0/fboHeight);
	
	float lumaTL	=	dot(luma,texture(colorTexture,passUVCoord.xy + (vec2(-1.0,-1.0) * texCoordOffset)).xyz);
	float lumaTR	=	dot(luma,texture(colorTexture,passUVCoord.xy + (vec2(1.0,-1.0) * texCoordOffset)).xyz);
	float lumaBL	=	dot(luma,texture(colorTexture,passUVCoord.xy + (vec2(-1.0,1.0) * texCoordOffset)).xyz);
	float lumaBR	=	dot(luma,texture(colorTexture,passUVCoord.xy + (vec2(1.0,1.0) * texCoordOffset)).xyz);
	float lumaM		=	dot(luma,texture(colorTexture,passUVCoord.xy).xyz);

	//calculate Blur direction - direction is zero if there is no edge here, because luminosity is similiar in this case
	vec2 dir;
	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
	dir.y =  ((lumaTL + lumaBL) - (lumaTR + lumaBR));
	
	float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR)*(FXAA_REDUCE_MUL * 0.25),FXAA_REDUCE_MIN); //prevents division by zero
	float inverseDirAdjustment = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
	
	dir = min(vec2(FXAA_SPAN_MAX,FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX,-FXAA_SPAN_MAX), dir * inverseDirAdjustment)) * texCoordOffset;
	
	vec3 result1 = (1.0/2.0) * (
	texture(colorTexture,passUVCoord.xy  + (dir * vec2(1.0/3.0 - 0.5))).xyz +
	texture(colorTexture,passUVCoord.xy  + (dir * vec2(2.0/3.0 - 0.5))).xyz);
	
	vec3 result2 = result1* (1.0/2.0) + (1.0/4.0) * (
	texture(colorTexture,passUVCoord.xy  + (dir * vec2(0.0/3.0 - 0.5))).xyz +
	texture(colorTexture,passUVCoord.xy  + (dir * vec2(3.0/3.0 - 0.5))).xyz);
	
	//test if result2 was too far sampled. if so - take result1 as result
	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL,lumaBR)));
	float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL,lumaBR)));
	float lumaResult2 = dot(luma,result2);
	
	if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)
		colorOutput = vec4(result1,1.0);
	else
		colorOutput = vec4(result2,1.0);	
}