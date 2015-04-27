#version 430 core

in float lifetimeparticle[];

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

//our camera position
uniform vec4 camPos;

//our rotation data
uniform int rotateLeft;
uniform float rotationSpeed;

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices=4) out;
out float lifetime;
out vec2 uv;

// Rotation matrix for rotating around any abitrary unit vector
mat4 rotationMatrix(vec3 axis, float angle)
{
	axis = normalize(axis);
	return mat4((1.0 - cos(angle)) * axis.x * axis.x + cos(angle),				(1.0 - cos(angle)) * axis.x * axis.y - axis.z * sin(angle),		(1.0 - cos(angle)) * axis.z * axis.x + axis.y * sin(angle),		0.0,
				(1.0 - cos(angle)) * axis.x * axis.y + axis.z * sin(angle),		(1.0 - cos(angle)) * axis.y * axis.y + cos(angle),				cos(angle) * axis.y * axis.z - axis.x * sin(angle),				0.0,
				(1.0 - cos(angle)) * axis.z * axis.x - axis.y * sin(angle),		(1.0 - cos(angle)) * axis.y * axis.z + axis.x * sin(angle),		(1.0 - cos(angle)) * axis.z * axis.z + cos(angle),				0.0,
				0.0,															0.0,															0.0,															1.0); 
} 

void main() {

	//pass through
	lifetime = lifetimeparticle[0];

	// scaling
	float s;

	if(useScaling == 1){
		float percentageLifetime = 1 - lifetime/fullLifetime;
		int upperBorder=0;
		do{
			upperBorder=upperBorder+2;
		}
		while( scalingData[upperBorder] < percentageLifetime && ((upperBorder <= scalingCount && particleMortal == 1) || (upperBorder < scalingCount && particleMortal == 0)));
		
		int lowerBorder = upperBorder-2;
		float pUpper = max(min((percentageLifetime - scalingData[lowerBorder]) / (scalingData[upperBorder] - scalingData[lowerBorder]), 1.0), 0.0);
		float scalingSize = (1-pUpper) * scalingData[lowerBorder+1] + pUpper * scalingData[upperBorder+1];
		
		s= scalingSize;
	}
	else{

		s=size;
	}
	
	// compute the look vector in view space
	vec4 pos = viewMatrix*vec4(gl_in[0].gl_Position.xyz,1.0);
	vec4 cameraPos= viewMatrix*vec4(camPos.xyz, 1.0);
	vec4 look = normalize(cameraPos-pos);

	//rotating direction
	int direction;
	if(rotateLeft == 0)
		direction = -1;
	else direction = 1;

	// rotation around the look vector depending on life time, rotation speed and rotation direction
	mat4 r=rotationMatrix(look.xyz, lifetime*rotationSpeed*direction);
	
	// computing the right and up vector and rotate them
	vec4 upHelp = normalize(viewMatrix*vec4(0.0,1.0,0.0,0.0));
	vec4 right = normalize(vec4(cross(look.xyz, upHelp.xyz),0.0))*r;
	vec4 up = vec4( cross( right.xyz, look.xyz),0.0);
	
	//creating the 4 billboard points depending on the scaling
	//down right
	uv = vec2(0,0);
	gl_Position = projectionMatrix* (pos +  vec4(right-up)*s);
	EmitVertex();

	//down left
	uv = vec2( 1,0);
	gl_Position = projectionMatrix * (pos + vec4(-right-up)*s);
	EmitVertex();

	//up right
	uv = vec2(0, 1);
	gl_Position = projectionMatrix * (pos + vec4(right+up)*s);
	EmitVertex();

	//up left
	uv = vec2( 1, 1);
	gl_Position = projectionMatrix *  (pos + vec4(-right+up)*s);
	EmitVertex();

	EndPrimitive();
}