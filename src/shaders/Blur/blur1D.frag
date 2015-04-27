#version 330

in vec2 passUV;

uniform sampler2D image;
uniform float blurstrength;

layout(location = 2) out vec4 colorOutput;
 
const float blurSize = 1.0/512.0; 
 
void main(void)
{
   vec4 sum = vec4(0.0);
   float blurAmount = blurSize * blurstrength;
   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture(image, vec2(passUV.x - 4.0*blurAmount, passUV.y)) * 0.05;
   sum += texture(image, vec2(passUV.x - 3.0*blurAmount, passUV.y)) * 0.09;
   sum += texture(image, vec2(passUV.x - 2.0*blurAmount, passUV.y)) * 0.12;
   sum += texture(image, vec2(passUV.x - blurAmount, passUV.y)) * 0.15;
   sum += texture(image, vec2(passUV.x, passUV.y)) * 0.16;
   sum += texture(image, vec2(passUV.x + blurAmount, passUV.y)) * 0.15;
   sum += texture(image, vec2(passUV.x + 2.0*blurAmount, passUV.y)) * 0.12;
   sum += texture(image, vec2(passUV.x + 3.0*blurAmount, passUV.y)) * 0.09;
   sum += texture(image, vec2(passUV.x + 4.0*blurAmount, passUV.y)) * 0.05;
 
   colorOutput = sum;
}