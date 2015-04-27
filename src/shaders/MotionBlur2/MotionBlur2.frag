#version 330 core

uniform mat4 modelMatrix;
uniform mat4 previousModelMatrix;
uniform mat4 viewMatrix;
uniform mat4 previousViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 previousProjectionMatrix;

in vec2 passUV;

layout(location = 2) out vec4 colorOutput;
 
uniform sampler2D ScreenTexture, depthTexture;
uniform float fWindowHeight;
uniform float fWindowWidth;
uniform float thresholdValue;

vec4 motionBlur()
{
	mat4 inverseModelProjection = mat4(transpose(inverse(projectionMatrix * viewMatrix * modelMatrix)));
	mat4 previousModelProjection = mat4(previousProjectionMatrix * previousViewMatrix * previousModelMatrix);
	vec4 tmp1,tmp2; 
	vec2 UV = passUV.xy;
	//Retrieve depth of pixel  
	float z = texture2D(depthTexture, UV).z;  
	
	//Simplified equation of GluUnproject
	vec4 currentPos = vec4( 2.0* (gl_FragCoord.x/fWindowHeight)  - 1.0, 2.0* (gl_FragCoord.y/fWindowWidth) - 1.0, 2.0*z -1.0 , 1.0);

	//Back into the worldSpace 
	tmp1 =  currentPos  * inverseModelProjection;  
	
	//Homogenous value 
	vec4 posInWorldSpace = tmp1/tmp1.w;  
	
	//Using the world coordinate, we transform those into the previous frame
	tmp2 =  previousModelProjection *posInWorldSpace ;  
	vec4 previousPos = tmp2/tmp2.w;  
	
	//Compute the frame velocity using the difference 
	vec2 velocity = ((currentPos - previousPos)/10.0).xy;

	//Get the initial color at this pixel.  
	vec4 originalColor = texture2D(ScreenTexture, UV);
	UV += velocity.xy;  
	for(int i = 1; i < 20.0; ++i)  
	{  
		//Sample the color buffer along the velocity vector.  
		vec4 currentColor = texture2D(ScreenTexture, UV);  
		//Add the current color to our color sum.  
		originalColor += currentColor;  
		UV.x += velocity.x;
		UV.y += velocity.y;
	}  
	//Average all of the samples to get the final blur color.  
	return originalColor / 20.0;  	
}

void main()
{	
	colorOutput = motionBlur();
}
