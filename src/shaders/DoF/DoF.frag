#version 330

in vec2 passUV;

uniform sampler2D tex;
uniform sampler2D depth;

uniform float focus_depth;
 
vec4 sampleBox(float u, float v, float size) {
  vec4 color = vec4(0.0,0.0,0.0,0.0);
  color += texture2D(tex, vec2(passUV.x - size,  passUV.y - size))  * 0.075;
  color += texture2D(tex, vec2(passUV.x,  passUV.y - size))  * 0.1;
  color += texture2D(tex, vec2(passUV.x + size,  passUV.y - size))  * 0.075;
  color += texture2D(tex, vec2(passUV.x - size,  passUV.y))  * 0.1;
  color += texture2D(tex, vec2(passUV.x,  passUV.y))  * 0.30;
  color += texture2D(tex, vec2(passUV.x + size,  passUV.y))  * 0.1;
  color += texture2D(tex, vec2(passUV.x - size,  passUV.y + size))  * 0.075;
  color += texture2D(tex, vec2(passUV.x,  passUV.y + size))  * 0.1;
  color += texture2D(tex, vec2(passUV.x + size,  passUV.y + size))  * 0.075;
  return color;
}

float clampZeroOne(float inputVal)
{
	if(inputVal < 0.0)
		return 0.0;
		
	if(inputVal > 1.0)
		return 1.0;
}
 
void main(void)
{
  float depth = texture2D(depth, passUV).r;
  float blur_amount = abs(depth-focus_depth)*20.0;
  if(depth < depth-focus_depth) { blur_amount *= 10.0; }
  blur_amount = clampZeroOne(blur_amount);
 
  vec4 baseColor = texture2D(tex,passUV);
  vec4 blurredColor = vec4(0.0,0.0,0.0,0.0);
 
  float blurSize = 0.005*blur_amount;
  blurredColor = 0.75*sampleBox(passUV.x, passUV.y, blurSize*0.5) + 0.25*sampleBox(passUV.x, passUV.y, blurSize*1.0);
 
  gl_FragColor = baseColor * (1.0 - blur_amount) + blurredColor * blur_amount;
}