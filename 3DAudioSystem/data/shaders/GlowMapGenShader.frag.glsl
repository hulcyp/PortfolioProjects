#version 330

uniform float time;

uniform sampler2D uColorTexture;
uniform sampler2D uDepthTexture;
uniform float uAspectRatio;
uniform int uScreenWidth;
uniform int uScreenHeight;

in vec2 vTexCoord;

out vec4 fragColor;

void main()
{
	vec4 texelColor = texture( uColorTexture, vTexCoord );

	float greenDiff = texelColor.r - texelColor.g;
	float blueDiff = texelColor.r - texelColor.b;

	if( greenDiff > 0.4 && blueDiff > 0.4 )
	{
		fragColor = vec4( 1.0, 0, 0, 1.0 );
	}
	else
	{
		fragColor = vec4( 0, 0, 0, 1 );
	}
}