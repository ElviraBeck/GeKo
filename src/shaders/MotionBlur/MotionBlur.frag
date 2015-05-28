#version 330 core
 
// INPUT
in vec4 v_vCorrectedPosScreenSpace;
in vec4 v_vCurrentPosScreenSpace;
in vec4 v_vPreviousPosScreenSpace;

// UNIFORMS
uniform sampler2D u_tColorTex; // In MotionBlurShader

// CONSTANTS
const int NUM_SAMPLES = 16;
const vec3 VZERO = vec3(0.0, 0.0, 0.0);

//#define VISUALIZE_VELOCITY

void main()
{
    vec2 vWindowPosition = v_vCorrectedPosScreenSpace.xy /
                           v_vCorrectedPosScreenSpace.w;
    vWindowPosition *= 0.5;
    vWindowPosition += 0.5;

    vec3 vScreenSpaceVelocity =
        ((v_vCurrentPosScreenSpace.xyz/v_vCurrentPosScreenSpace.w) -
         (v_vPreviousPosScreenSpace.xyz/v_vPreviousPosScreenSpace.w)) * 0.5;

#ifdef VISUALIZE_VELOCITY
    // 0.5 is used to scale the screen space velocity of the pixel from [-1,1]
    // to [0,1]
    gl_FragColor = vec4(vScreenSpaceVelocity.x,
                        vScreenSpaceVelocity.y,
                        vScreenSpaceVelocity.z,
                        1.0);
#else
    float fWeight = 0.0;
    vec3 vSum = VZERO;
    float fNumSamples = float(NUM_SAMPLES);

    for(int k = 0; k < NUM_SAMPLES; ++k) 
    {
        float fOffset = float(k) / (fNumSamples - 1.0);
        vec4 vSample = texture2D(u_tColorTex,
            vWindowPosition + (vScreenSpaceVelocity.xy * fOffset));

        // The unblurred geometry is rendered with alpha=1 and the background
        // with alpha=0.  So samples that hit the geometry have alpha=1. For
        // accumulation and blur purposes, ignore samples that hit the
        // background.
        vSum += (vSample.rgb * vSample.a);
        fWeight += vSample.a;
    }

    if (fWeight > 0.0)
        vSum /= fWeight;

    float fAlpha = fWeight / fNumSamples;

    gl_FragColor = vec4(vSum, fAlpha);
#endif
}
