#version 330

in vec2 passUV;

//layout(location = 2) out vec4 colorOutput;
  
uniform sampler2D tex;
uniform float blurstrength;
uniform float bluramount = 1.5f;

vec2 texcoord = passUV;

layout(location = 2) out vec4 colorOutput;

void main()
{

vec3 color = texture2D(tex, passUV).rgb;

// generate vingette falloff factor
vec2 powers = pow(abs(passUV.xy - 0.5),vec2(2.0));
float gradient = smoothstep(0.49-0.6, 0.49+0.6, powers.x+powers.y);
gradient *= blurstrength;

// use vingette as blur factor
vec4 col = vec4(0.0);
for ( float x = -bluramount + 1.0; x < bluramount; x += 1.0 )
{
for ( float y = -bluramount + 1.0; y < bluramount; y += 1.0 )
{
col+=texture2D(tex,passUV+vec2(gradient*x,gradient*y));
}
}
col /= ((bluramount*2.0)-1.0)*((bluramount*2.0)-1.0);

colorOutput = col;
}
