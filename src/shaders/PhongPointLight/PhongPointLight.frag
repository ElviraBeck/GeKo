#version 330 core

in vec4 passPosition;
in vec3 passNormal;

uniform mat4 viewMatrix;

uniform vec4 l_col;
uniform vec4 l_pos;
uniform float shininess;

uniform vec4 l_ambient;
uniform vec4 l_diffuse;
uniform vec4 l_specular;
out vec4 fragmentColor;

vec3 lightVector;

void main(){

	//ambeint
	vec4 ambient = l_ambient;

	//diffuse
	lightVector = normalize(l_pos.xyz - passPosition.xyz);
    float cos_phi = max(dot(passNormal, lightVector), 0.0f);
	vec4 diffuse = l_diffuse * cos_phi * l_col;

	//specular
	vec3 eye = normalize(-passPosition.xyz);
	vec3 reflection = normalize(reflect(-lightVector, normalize(passNormal)));
    float cos_psi = pow(max(dot(reflection, eye)+1.0, 0.0), shininess); //BUG!! specular is always null. "+1.0" is an uncorrect Bugfix
	vec4 specular = l_specular * l_col * cos_psi;

	//color
	fragmentColor = vec4(ambient+diffuse+specular);
}