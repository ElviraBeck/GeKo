#version 330 core

#define HW_PCF

uniform int useTexture;
uniform int useNormalMap;
uniform int useHeightMap;
uniform int useHeightMapShadows;
uniform int useShadowMap;

uniform int shadowMode;

uniform sampler2D fboTexture;
uniform sampler2D normalMap;
uniform sampler2D heightMap;

uniform sampler2D depthTexture;

in vec3 passSkyboxTexCoord;
uniform samplerCube cubeMap;
uniform int renderSkybox;

uniform float fWindowHeight;
uniform float fWindowWidth;
uniform float thresholdValue;

uniform mat4 modelMatrix;
uniform mat4 previousModelMatrix;
uniform mat4 viewMatrix;
uniform mat4 previousViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 previousProjectionMatrix;

uniform float parallaxScale;
uniform float parallaxBias;

uniform struct 
{
    vec3 diffuse;
	vec3 specular;
    float shininess;
	float alpha;
} mat;

uniform struct 
{
  vec4 pos;    //pos.w = 0 dir. light, pos.w = 1 point light
  vec3 col;
  vec3 spot_direction; 
  float spot_exponent; 
  float spot_cutoff;  
} light;

uniform vec3 lightAmbient;
uniform float fDepthBias = 0.005f;

in vec4 passPosition;
in vec3 passNormal;
in vec2 passUV;
in vec4 passShadowCoord;

in vec3 passWorldNormal;
in mat3 normalMatrix;

layout(location = 0) out vec4 positionOutput;
layout(location = 1) out vec4 normalOutput;
layout(location = 2) out vec4 colorOutput;

float sampleShadowMap(sampler2D shadowMap, vec3 shadowCoord) 
{
	  if (texture(shadowMap, shadowCoord.xy).z < shadowCoord.z - fDepthBias) 
        return 0.3f; 
	return 1.0f;
}

float sampleShadowMap_Linear(sampler2D shadowMap, vec3 shadowCoord) 
{
	vec2 texelSize	= vec2(1.0f/1024.0f, 1.0f/1024.0f);
	vec2 pixelPos	= (shadowCoord.xy/texelSize) + vec2(0.5f);
	vec2 fracPart	= fract(pixelPos);
	vec2 startTexel = (pixelPos - fracPart) * texelSize;

	float blTexel = sampleShadowMap(shadowMap, vec3(startTexel,shadowCoord.z));
	float brTexel = sampleShadowMap(shadowMap, vec3(startTexel + vec2(texelSize.x,0.0),shadowCoord.z));
	float tlTexel = sampleShadowMap(shadowMap, vec3(startTexel + vec2(0.0,texelSize.y),shadowCoord.z));
	float trTexel = sampleShadowMap(shadowMap, vec3(startTexel + vec2(texelSize.x,texelSize.y),shadowCoord.z));

	float mixA = mix(blTexel,tlTexel,fracPart.y);
	float mixB = mix(brTexel,trTexel,fracPart.y);

	return mix(mixA,mixB,fracPart.x);
}

float sampleShadowMap_PCF(sampler2D shadowMap, vec3 shadowCoord) 
{

	const float f_PCFsize = 5.0f;				//3x3 Area, 5x5...
	const float start = (f_PCFsize-1.0f)/2.0f;
	const float f_PCFsizeSquared = f_PCFsize * f_PCFsize;

	vec2 texelSize	= vec2(1.0f/1024.0f, 1.0f/1024.0f);
	float result = 0.0f;

	for(float y = -start; y <= start; y += 1.0f) {
		for(float x = -start; x <= start; x += 1.0f) {
				vec2 coordsOffset = vec2(x,y)*texelSize;
				vec3 samplingPoint = vec3(shadowCoord.x + coordsOffset.x,shadowCoord.y + coordsOffset.y,shadowCoord.z);
				result += sampleShadowMap(shadowMap,samplingPoint);
			}
		}

		return result/f_PCFsizeSquared;
}


float sampleShadowMap_PCF_linear(sampler2D shadowMap, vec3 shadowCoord) 
{

	const float f_PCFsize = 5.0f;				//3x3 Area, 5x5...
	const float start = (f_PCFsize-1.0f)/2.0f;
	const float f_PCFsizeSquared = f_PCFsize * f_PCFsize;

	vec2 texelSize	= vec2(1.0f/1024.0f, 1.0f/1024.0f);
	float result = 0.0f;

	for(float y = -start; y <= start; y += 1.0f) {
		for(float x = -start; x <= start; x += 1.0f) {
				vec2 coordsOffset = vec2(x,y)*texelSize;
				vec3 samplingPoint = vec3(shadowCoord.x + coordsOffset.x,shadowCoord.y + coordsOffset.y,shadowCoord.z);
				result += sampleShadowMap_Linear(shadowMap,samplingPoint);
			}
		}

		return result/f_PCFsizeSquared;
}

vec4 shadowMapping(vec4 baseColor)
{
	vec3 lightVector;
	float spot;
	vec3 diffuse_color;
	float diffuse_alpha;

	// Diffuse
	vec3 light_camcoord = (viewMatrix * light.pos).xyz;
    if (light.pos.w > 0.001f)
		lightVector = normalize( light_camcoord - passPosition.xyz);
    else
		lightVector = normalize(light_camcoord);
    float cos_phi = max( dot( passNormal, lightVector), 0.0f);

    // Specular
    vec3 eye = normalize(-passPosition.xyz);    
    vec3 reflection = normalize( reflect( -lightVector, passNormal));
    float cos_psi_n = pow( max( dot( reflection, eye), 0.0f), mat.shininess);

    // Spotlight	
	float cos_phi_spot = max( dot( -lightVector, mat3(viewMatrix) * light.spot_direction), 0.0f);
	if( cos_phi_spot >= cos( light.spot_cutoff))
		spot = pow( cos_phi_spot, light.spot_exponent);
	else
		spot = 0.0f;

	// Textures 
	if (useTexture != 0)
	{
		diffuse_color = baseColor.rgb;
		diffuse_alpha = baseColor.a;
	}
	else
	{
		diffuse_color = mat.diffuse;
		diffuse_alpha = mat.alpha;
	}
	
	// Shadow
	float inShadow = 1.0;
	vec3 shadowCoord = passShadowCoord.xyz / passShadowCoord.w;

	//NORMAL
	if(shadowMode == 0) {
		inShadow = sampleShadowMap(depthTexture,shadowCoord);
	}

	//LINEAR FILTERING
	else if(shadowMode == 1) {
	inShadow = sampleShadowMap_Linear(depthTexture,shadowCoord);
	}

	//PCF FILTERING
	else if(shadowMode == 2) {
	inShadow = sampleShadowMap_PCF(depthTexture,shadowCoord);
	}

	//PCF LINEAR FILTERING
	else if (shadowMode == 3) {
	inShadow = sampleShadowMap_PCF_linear(depthTexture,shadowCoord);
	}

	vec4 fragmentColor = vec4(1.0f,1.0f,1.0f,1.0f);

	// All together 
	fragmentColor.rgb = diffuse_color * lightAmbient;
	fragmentColor.rgb += spot * diffuse_color * cos_phi * light.col;
	//fragmentColor.rgb += spot * mat.specular * cos_psi_n * light.col;
	fragmentColor.rgb *= inShadow;
	fragmentColor.a = diffuse_alpha;

	return fragmentColor;
}

 
void main(){  

	positionOutput = passPosition;
	normalOutput = vec4(normalize(passNormal), 1);
	
	if(renderSkybox != 0)
	{
		colorOutput = texture(cubeMap, passSkyboxTexCoord);	
	}else
	{
	if (useTexture != 0)
	{
		colorOutput = texture(fboTexture,passUV);
	}
	
	else
	{
		colorOutput = vec4(1.0f,1.0f,1.0f,1.0f);
	}
		
	vec2 finalUV = passUV;
	vec4 heightMapShadowValue = vec4(1,1,1,1);
	
	if(useHeightMap != 0)
	{
		vec3 eyeVector = normalize(-passPosition).xyz;	
		
		float height = texture(heightMap,passUV).r * parallaxScale + ((-parallaxScale/2.0f) + (parallaxScale/2.0f) * parallaxBias);

		finalUV = passUV.xy + (height * eyeVector.xy);
		
		if(useHeightMapShadows != 0)
		{		
			heightMapShadowValue = vec4(texture(heightMap,passUV).rgb,1.0) + vec4(0.2,0.2,0.2,1.0);
			colorOutput = vec4(colorOutput.rgb, 1.0f) * heightMapShadowValue;
		}
	}
	
	if (useShadowMap != 0)
	{
		colorOutput = shadowMapping(colorOutput);
	}

	if (useNormalMap != 0)
	{
		vec3 Normal   = vec3(normalize(normalMatrix * passWorldNormal));
		vec3 Tangent  = normalize(normalMatrix[0]); 
		vec3 Binormal = normalize(normalMatrix[1]);
		
		mat3 tangentToWorldSpace = mat3(Tangent.x, Binormal.x, Normal.x,
										Tangent.y, Binormal.y, Normal.y,
										Tangent.z, Binormal.z, Normal.z);
						   
		vec3 calcNormal = vec3((texture(normalMap,finalUV).rgb * 2.0 - 1.0)*tangentToWorldSpace);
		normalOutput =  vec4(calcNormal,1.0);
	}
	}
}
