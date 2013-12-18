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

uniform sampler2D uBufferPassTextureColor0;
uniform int uOrientation;
uniform int uBlurAmount;
uniform float uBlurScale;
uniform float uBlurStrength;

in vec2 vTexCoord;

out vec4 fragColor;

float pixelWidth = 1.0 / uScreenWidth;
float pixelHeight = 1.0 / uScreenHeight;

//float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );
float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.2702702703 );
//float offset[3] = float[]( 0.4, 5.3846153846, 10.2307692308 );
//float weight[3] = float[]( 0.5, 0.4, 0.9 );
//float weight[3] = float[]( 1.5, 1.4, 1.9 );
float offset[3] = float[]( 0.4, 5.3846153846, 10.2307692308 );

float getLuminosity( in vec3 color )
{
	vec3 luminosityFactors = vec3( 0.2126, 0.7152, 0.0722 );
	return dot( color, luminosityFactors );
}

float Gaussian( float x, float deviation )
{
	return ( 1.0 / sqrt( 2.0 * 3.141592 * deviation ) ) * exp( -( (x * x ) / ( 2.0 * deviation ) ) );
}

vec3 gaussianBlur()
{
	vec3 texColor = vec3( 1.0, 0.0, 0.0 );

	if( uOrientation == 0 )
	{
		//if( vTexCoord.x < 500*pixelWidth - 0.01 )
		//{
			vec2 uv = vTexCoord.xy;
			texColor = texture( uColorTexture, uv ).rgb * weight[0];
			for( int i = 1; i < 3; ++i )
			{
				texColor += texture( uColorTexture, uv + vec2( 0, offset[i] * pixelHeight ) ).rgb * weight[i];
				texColor += texture( uColorTexture, uv - vec2( 0, offset[i] * pixelHeight ) ).rgb * weight[i];
				//float luminosity = getLuminosity( texColor );
				//texColor = vec3( luminosity, luminosity, luminosity );

			}
		//}
		//else if( vTexCoord.x < 500*pixelWidth - 0.001 )
		//{
		//	texColor = vec3( 1.0, 0, 0 );
		//}
		//else
		//{
		//	texColor = texture( uColorTexture, vTexCoord.xy ).rgb;	
		//}
	}
	else
	{
		vec2 uv = vTexCoord.xy;
		texColor = texture( uColorTexture, uv ).rgb * weight[0];
		for( int i = 1; i < 3; ++i )
		{
			texColor += texture( uColorTexture, uv + vec2( offset[i] * pixelHeight, 0 ) ).rgb * weight[i];
			texColor += texture( uColorTexture, uv - vec2( offset[i] * pixelHeight, 0 ) ).rgb * weight[i];
		}
	}
	return texColor;
}

void main()
{
	float halfBlur = float( uBlurAmount ) * 0.5;
	vec4 color = vec4( 0 );
	vec4 texColor = vec4( 0 );

	float deviation = halfBlur * 0.35;
	deviation *= deviation;
	float strength = 1.0 - uBlurStrength;

	if( uOrientation == 0 )
	{
		// Horizontal blur
		for( int i = 0; i < 10; ++i )
		{
			if( i >= uBlurAmount )
				break;

			float offset = float(i) - halfBlur;
			texColor = texture( uColorTexture, vTexCoord + vec2( offset + pixelWidth * uBlurScale, 0.0 ) ) * Gaussian( offset * strength, deviation );
			color += texColor;
		}
	}
	else
	{
		// Vertical blur
		for( int i = 0; i < 10; ++i )
		{
			if( i >= uBlurAmount )
				break;

				float offset = float(i) - halfBlur;
				texColor = texture( uColorTexture, vTexCoord + vec2( 0.0, offset + pixelHeight * uBlurScale ) ) * Gaussian( offset * strength, deviation );
				color += texColor;
		}	
	}

	fragColor = clamp( color, 0.0, 1.0 );
	fragColor.a = 1.0;
	fragColor = vec4( gaussianBlur(), 1.0 );
}