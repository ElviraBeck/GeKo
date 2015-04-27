#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

// OUTPUT
out vec4 v_vCorrectedPosScreenSpace;
out vec4 v_vCurrentPosScreenSpace;
out vec4 v_vPreviousPosScreenSpace;

// UNIFORMS
uniform  mat4 u_mProjectionMat;    // In BaseProjectionShader
uniform  mat4 u_mViewMat;          // In MotionBlurES2Shader
uniform  mat4 modelMatrix;			// In MotionBlurES2Shader
uniform  mat4 previousModelMatrix; // In MotionBlurES2Shader
uniform float u_fStretchScale;     // In MotionBlurES2Shader

void main()
{
	mat4 u_mNormalMat = mat4(transpose(inverse(u_mViewMat * modelMatrix)));
    vec4 vCurrPosWorldSpace = modelMatrix  * position;
    vec4 vPrevPosWorldSpace = previousModelMatrix * position;
    
    vec3 vMotionVecWorldSpace =
        vCurrPosWorldSpace.xyz - vPrevPosWorldSpace.xyz;
    vec3 vNormalVecWorldSpace = normalize(mat3(u_mNormalMat) * normal);

    // Scaling the stretch.
    vec4 vStretchPosWorldSpace = vCurrPosWorldSpace;
    vStretchPosWorldSpace.xyz -= (vMotionVecWorldSpace * u_fStretchScale);

    v_vCurrentPosScreenSpace =
        u_mProjectionMat * u_mViewMat * vCurrPosWorldSpace;
    v_vPreviousPosScreenSpace =
        u_mProjectionMat * u_mViewMat * vPrevPosWorldSpace;
    vec4 vStretchPosScreenSpace =
        u_mProjectionMat * u_mViewMat * vStretchPosWorldSpace;

    v_vCorrectedPosScreenSpace =
        dot(vMotionVecWorldSpace, vNormalVecWorldSpace) > 0.0 ?
            v_vCurrentPosScreenSpace :
            vStretchPosScreenSpace;

    gl_Position = v_vCorrectedPosScreenSpace;
}