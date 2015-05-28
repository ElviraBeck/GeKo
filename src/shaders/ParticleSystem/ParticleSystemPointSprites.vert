#version 430 core

layout (location = 0) in vec4 position;	

//our matrixes
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//our scaling Data
uniform int useScaling;
uniform int scalingCount;
uniform float scalingData[32]; //first moment, then size
uniform float size;
uniform float fullLifetime;
uniform int particleMortal;

out float lifetime;

void main(){
	//our needed var
	int direction;
	float scalingSize;

	//world space particles
	vec4 pos = projectionMatrix * viewMatrix * vec4(position.xyz, 1.0);
	
	//scaling
	float remainLifetime = position.w;
	if(useScaling == 1){
		float percentageLifetime = 1 - remainLifetime/fullLifetime;
		int upperBorder=0;
		do{
			upperBorder=upperBorder+2;
		}
		while( scalingData[upperBorder] < percentageLifetime && ((upperBorder <= scalingCount && particleMortal == 1) || (upperBorder < scalingCount && particleMortal == 0)));
		int lowerBorder = upperBorder-2;
		float pUpper = max(min((percentageLifetime - scalingData[lowerBorder]) / (scalingData[upperBorder] - scalingData[lowerBorder]), 1.0),0.0); 
		scalingSize = (1-pUpper) * scalingData[lowerBorder+1] + pUpper * scalingData[upperBorder+1];
	}
	else{
		scalingSize = size;
	}

	//Point Size
	gl_PointSize = (1.0 - pos.z / pos.w) * (2500.0 * scalingSize);

	//Output
	lifetime = remainLifetime;
	gl_Position = pos;
}