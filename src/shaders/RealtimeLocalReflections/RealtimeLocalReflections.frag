#version 330

in vec2 passUVCoord;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D colorTexture;
uniform sampler2D depthBuffer;

uniform mat4 projectionMatrix;

uniform int screenWidth;
uniform int screenHeight;

uniform float zNear;
uniform float zFar;

uniform float reflectivity;

layout(location = 2) out vec4 colorOutput;

float normalizeDepth(float depth)
{
	return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

vec3 convertCameraToScreen(in vec3 cameraSpace)
{
	vec4 hom = projectionMatrix * vec4(cameraSpace,1.0);
	vec3 can = hom.xyz / hom.w;
	//we want to sample from a texture -> bring into range [0,1]
	return vec3(0.5 * can + 0.5);
}

vec4 computeReflection()
{
	bool FADE2VIEWER = true;
	bool FADE2EDGE = true;
	float fade2EdgeDistance = 0.2;
	float stepSize = 0.02;
				
	vec4 color = vec4(0.0,0.0,0.0,1);
	int count = 0;
	int depth = 0;
	int maxDepth = 3;

	int numRefine = 0;
	float stepRefine = 0.7;
	int maxRefine = 3;
	
	//Compute reflection vector in screen space for each pixel
	vec2 screenPos = passUVCoord;
	vec3 posCameraSpace = texture(positionTexture,screenPos).xyz;
	vec3 normCameraSpace = texture(normalTexture,screenPos).xyz;
	vec3 reflectionCameraSpace = reflect(normalize(posCameraSpace),  normalize(normCameraSpace));
	
	vec3 posReflCameraSpace = posCameraSpace + reflectionCameraSpace;
	vec3 posReflScreenSpace = convertCameraToScreen(posReflCameraSpace);
	vec3 posScreenSpace = convertCameraToScreen(posCameraSpace);
	vec3 reflectionScreenSpace = posReflScreenSpace - posScreenSpace;

	reflectionScreenSpace = stepSize * normalize(reflectionScreenSpace);

	vec3 oldPos = posScreenSpace + reflectionScreenSpace;
	vec3 currentPos = oldPos + reflectionScreenSpace;
	  
	//Raymarch along reflection vector
	while(depth < maxDepth)
	{
		while(count < 50)
		{
			//stop ray when reaching the end of screen space
			if( currentPos.x < 0.0 || currentPos.x > 1.0 ||
				currentPos.y < 0.0 || currentPos.y > 1.0 ||
				currentPos.z < 0.0 || currentPos.z > 1.0)
				break;
				
			//Sample depth and check if ray depth is within threshold to scene depth
		
			float currentDepth = normalizeDepth(currentPos.z);
			float sampleDepth = normalizeDepth(texture(depthBuffer, currentPos.xy).x);
						
			if(currentDepth >= sampleDepth && currentDepth - sampleDepth < length(reflectionScreenSpace))
			{
				reflectionScreenSpace *= stepRefine;
				currentPos = oldPos;
				numRefine++;
				if(numRefine >= maxRefine)
				{
					//read Normal from new position and calc orientation
					vec3 posNormal = texture(normalTexture,currentPos.xy).xyz;
					
					//color fetch for new position
					color = texture(colorTexture,currentPos.xy);
					break;
				}
			}			
			oldPos = currentPos;
			currentPos = oldPos + reflectionScreenSpace;
			count++;
		}
		depth++;
	}
	
	//fadeToViewer
	float faceViewerFactor = (dot(reflectionCameraSpace,normCameraSpace));
	
	if(FADE2VIEWER)
	{
		if(faceViewerFactor <= 0.0)
			//angle is bigger than 90° - full reflection strength
			faceViewerFactor = 1.0;
		else
			//if the angle is smaller than 90° there is a good chance,
			//that the reflected vector is facing the viewer and that there is no reflection information
			//we slowly fade out the reflection
			faceViewerFactor = 1.0-faceViewerFactor;
	}
	else
	{
		faceViewerFactor = 1.0;
	}
	
	float edgeFactor = 1.0;
	
	if(FADE2EDGE)
	{		
		if(screenPos.x <= fade2EdgeDistance || screenPos.y <= fade2EdgeDistance)
			edgeFactor = min(screenPos.x,screenPos.y)/fade2EdgeDistance;
		
		else if(screenPos.x >= 1-fade2EdgeDistance || screenPos.y >= 1-fade2EdgeDistance)
			edgeFactor = (1-max(screenPos.x,screenPos.y))/fade2EdgeDistance;
	}
	return color * faceViewerFactor * edgeFactor;
}


void main()
{
	colorOutput = texture(colorTexture, passUVCoord);
	if(reflectivity > 0)
		colorOutput = reflectivity*computeReflection() + (1.0 - reflectivity)*colorOutput;
	colorOutput.w = 1.0;
}