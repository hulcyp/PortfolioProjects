#version 330

uniform float time;

uniform sampler2D uColorTexture;
uniform sampler2D uDepthTexture;
uniform float uAspectRatio;
uniform int uScreenWidth;
uniform int uScreenHeight;
uniform sampler2D uNoiseTexture;

in vec2 vTexCoord;
in vec3 vModelPosition;

out vec4 fragColor;

void main()
{	
	vec2 centerTex = vec2( 0.5, 0.5 );

	vec2 delta = vTexCoord - centerTex ;
	float sqrDistance = dot( delta, delta );
	fragColor = texture( uColorTexture, vTexCoord );
	float signX = sign( delta.x );
	float signY = sign( delta.y );

	
	
	float sampleX = signY * 0.1*time - delta.y + 0.37*sin( 1.0 - 0.2 * delta.x );
	float sampleY = signX * 0.1*time - delta.x + 0.47*sin( 1.0 - .4 * delta.y );
	vec4 noiseSample = texture( uNoiseTexture, vec2( sampleX, sampleY ) );
	fragColor += vec4( 0, 2.0 * sqrDistance * noiseSample.x, 0, 0 );
	

}