#version 330

in vec2 passUV;

uniform sampler2D bgl_RenderedTexture;

layout(location = 2) out vec4 colorOutput;

void main()
{
vec4 sum = vec4(0);
vec2 texcoord = passUV;
int j;
int i;

for( i= -4 ;i < 4; i++)
{
for (j = -4; j < 4; j++)
{
sum += texture2D(bgl_RenderedTexture, texcoord + vec2(j, i)*0.0015) * 0.20; 
}
}

colorOutput = sum*sum*0.003+texture2D(bgl_RenderedTexture, texcoord);
}