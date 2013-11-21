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
	float offset = 20.0;
	float difference = 1000.0;

	vec4 normalSampleX1 = texture( uDepthTexture, vTexCoord + vec2( offset, 0 ) );
	vec4 normalSampleX2 = texture( uDepthTexture, vTexCoord - vec2( offset, 0 ) );
	vec4 normalSampleY1 = texture( uDepthTexture, vTexCoord + vec2( 0, offset ) );
	vec4 normalSampleY2 = texture( uDepthTexture, vTexCoord - vec2( 0, offset ) );

	vec4 depthSample = texture( uDepthTexture, vTexCoord );

	float differenceX1 = abs( depthSample.x - normalSampleX1.x );
	float differenceX2 = abs( depthSample.x - normalSampleX2.x );
	float differenceY1 = abs( depthSample.x - normalSampleY1.x );
	float differenceY2 = abs( depthSample.x - normalSampleY2.x );

	if( differenceX1 < difference ||
		differenceX2 < difference ||
		differenceY1 < difference ||
		differenceY2 < difference )
	{
		fragColor = vec4( 0, 0, 0, 1 );		
	}
	else
	{
		fragColor = texture( uColorTexture, vTexCoord );
	}

}