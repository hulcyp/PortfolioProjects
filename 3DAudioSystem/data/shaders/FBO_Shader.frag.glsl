#version 330

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;

uniform sampler2D uColorTexture;
uniform sampler2D uDepthTexture;
uniform float uAspectRatio;
uniform int uScreenWidth;
uniform int uScreenHeight;

in vec4 vColor;
in vec2 vTexCoord;
in vec3 vModelPosition;

out vec4 fragColor;

float pixelWidth = 1.0 / uScreenWidth;
float pixelHeight = 1.0 / uScreenHeight;

//float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );
float offset[3] = float[]( 0.4, 5.3846153846, 10.2307692308 );
//float weight[3] = float[]( 0.5, 0.4, 0.9 );

float getLuminosity( in vec3 color )
{
	vec3 luminosityFactors = vec3( 0.2126, 0.7152, 0.0722 );
	return dot( color, luminosityFactors );
}

vec3 gaussianBlur()
{
	vec3 texColor = vec3( 1.0, 0.0, 0.0 );

	if( vTexCoord.x < 500*pixelWidth - 0.01 )
	{
		vec2 uv = vTexCoord.xy;
		texColor = texture( uColorTexture, uv ).rgb * weight[0];
		for( int i = 1; i < 3; ++i )
		{
			texColor += texture( uColorTexture, uv + vec2( 0, offset[i] * pixelHeight ) ).rgb * weight[i];
			texColor += texture( uColorTexture, uv - vec2( 0, offset[i] * pixelHeight ) ).rgb * weight[i];
			float luminosity = getLuminosity( texColor );
			texColor = vec3( luminosity, luminosity, luminosity );

		}
	}
	else if( vTexCoord.x < 500*pixelWidth - 0.001 )
	{
		texColor = vec3( 1.0, 0, 0 );
	}
	else
	{
		texColor = texture( uColorTexture, vTexCoord.xy ).rgb;	
	}

	return texColor;
}

void main()
{

	vec3 texColor = gaussianBlur();
	float luminosity = getLuminosity( texColor );


	fragColor = vec4( texColor, 1.0 );
	//fragColor = vec4( luminosity, luminosity, luminosity, 1.0 );
}