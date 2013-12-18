#version 330

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;
uniform sampler2D uTexels;
uniform float uFontHeight;
uniform sampler2D uNoiseTexture;

in vec4 vColor;
in vec2 vTexCoord;
in vec3 vModelPosition;
in float vNormalizedHeight;

out vec4 fragColor;


void main()
{	
	float noiseXSample1 = 0.4*time - 0.01*vModelPosition.y + 0.37*sin( 1.0 - 0.2*vModelPosition.x );
	float noiseXSample2 = 0.37*time - 0.05*vModelPosition.y + 0.03*sin( 1.0 + vModelPosition.x );
	
	float noiseXSample3 = 0.57*time - 0.03*vModelPosition.x + 2*sin( 1.0 - vModelPosition.y );
	float noiseXSample4 = 0.18*time - 0.25*vModelPosition.x + sin( 1.0 + vModelPosition.y );

	vec4 noiseSample1 = texture( uNoiseTexture, vec2( noiseXSample1, 0.1 ) );
	vec4 noiseSample2 = texture( uNoiseTexture, vec2( noiseXSample2, 0.2 ) );
	vec4 noiseSample3 = texture( uNoiseTexture, vec2( noiseXSample3, 0.3 ) );
	vec4 noiseSample4 = texture( uNoiseTexture, vec2( noiseXSample4, 0.4 ) );

	vec4 waveColor = 0.3*noiseSample1 + 0.05*noiseSample2 + 0.5*noiseSample3 + 0.1*noiseSample4;
	
	vec4 gradient = vec4( 1.0, 1.0, 1.0, 1.0 - vNormalizedHeight );

	float red = waveColor.r;
	red = red * red;
	float green = red * red;
	float blue = green * green * green;
	red += red * red;

	vec4 fireColor = vec4( red, green, blue, 1.0 );

	float constraint = sin( vModelPosition.x );
	float noiseXSample1Normalized = mod( noiseXSample1, 1.0 );

	if( fireColor.r > 0.8 && noiseXSample1Normalized >  constraint ) 
		discard;

	vec4 texColor = texture( uTexels, vTexCoord );

	fragColor = texColor * gradient * fireColor;
}