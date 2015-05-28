#version 330 core

uniform sampler2D colortexture;
uniform sampler2D normaltexture;

uniform vec4 pos;
uniform vec4 col;

in vec3 passLightVector;
in vec3 passEyeVector;
in vec2 passUV;

out vec4 fragmentColor;

void main(){
    
	int useColorTexture = 1;
	vec3 mat_diffuse = vec3(0.5f, 0.5f, 0.5f);
	vec3 mat_specular = vec3(0.3f, 0.3f, 0.3f);
	float mat_shininess = 120.0f;

	//RedMaterial
	//vec3 mat_diffuse = vec3(1.0f, 0.0f, 0.0f);
	//vec3 mat_specular = vec3(1.0f, 1.0f, 1.0f);

	/*struct Material //replacement for Material
	{   
		vec3 diffuse;
		vec3 specular;
	    float shininess;
	} mat;*/

	vec3 lightAmbient = vec3(0.1,0.1,0.1);

    /***************** Diffuse ******************************************************/
    // local normal, in tangent space
    vec3 normal = texture(normaltexture,passUV).rgb * 2.0 - 1.0;

    // direction of the light (from the fragment to the light) in tangent space
    vec3 lightVector = normalize(passLightVector);

    //compute the diffuse lighting factor
    float cos_phi = max(dot(normal, lightVector), 0);

    /***************** Specular *****************************************************/
    // compute the normalized reflection vector using GLSL's built-in reflect() function
    vec3 reflection = normalize(reflect(-lightVector, normal));

    // eye vector in tangent space
    vec3 eyeVector = normalize(passEyeVector);

    // compute the specular lighting factor
    float cos_psi_n = pow(max(dot(reflection, eyeVector), 0.0f), mat_shininess);

    /***************** Material properties ******************************************/
    vec3 diffuse_color;
    if (useColorTexture != 0)
        diffuse_color = texture(colortexture, passUV).rgb;
    else
        diffuse_color = mat_diffuse;
    
    /***************** All together *************************************************/
    fragmentColor.rgb = diffuse_color * lightAmbient;
    fragmentColor.rgb += diffuse_color * cos_phi * col.rgb;
    fragmentColor.rgb += mat_specular * cos_psi_n * col.rgb;
    fragmentColor.a = 1.0;

}