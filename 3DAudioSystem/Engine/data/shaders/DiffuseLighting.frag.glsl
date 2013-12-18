#version 330
#pragma optionNV unroll all

struct Attenuation
{
	float start;
	float end;
};

struct Aperture
{
	float innerConeDot;
	float outerConeDot;
};

struct Light
{
	vec3 position;
	vec4 color;
	vec3 direction;
	float ambientFactor;
	vec4 attenuationType;
	Aperture aperture;
	Attenuation attenuation;
};

const int MAX_NUM_LIGHTS = 8;

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;

uniform vec3 cameraPos;
uniform Light uLights[ MAX_NUM_LIGHTS ];

uniform sampler2D uDiffuseMap;
uniform sampler2D uNormalMap;
uniform sampler2D uEmissiveMap;
uniform int uUseNormalMap;
uniform int uUseEmissiveMap;

//debugging
uniform int uUnlit;

in vec4 vColor;
in vec2 vTexCoord0;
in vec4 vScreenPos;
in vec3 vCameraToVertex;
in vec3 vWorldPosition;
in vec3 vNormal;

in vec3 vLightDirFromVertexTBN[ MAX_NUM_LIGHTS ];
in float vLightLength[ MAX_NUM_LIGHTS ];
in vec3 vLightDirTBN[ MAX_NUM_LIGHTS ];

out vec4 fragColor;

vec3 calcRGBEncodingForXYZ( vec3 xyz )
{
	return xyz * .5 + vec3( .5 );
}

vec3 transformColorToNormal( in vec2 texCoord )
{	
	vec4 col = texture( uNormalMap, texCoord );
	return 2.0*( col.rgb ) - vec3( 1.0 );
}

vec4 getDirectLight( in Light light, in float attenuationFactor, in vec3 normal, in vec3 lightDirFromVertexTBNNormalized, in vec3 lightDirTBNNormalized, in vec2 texCoord )
{
	float unAttenuated = max( 0.0, dot( normal, -lightDirTBNNormalized ) );
	float directIntensity = max( 0.0, unAttenuated * attenuationFactor ) * ( light.color.a ) + max( 0.0, unAttenuated ) * ( 1.0 - light.color.a );
	return vec4( texture( uDiffuseMap, texCoord ).rgb * directIntensity * light.color.rgb, 1.0 );
}

vec4 getLighting( in vec2 texCoord )
{
	vec4 totalDirect;
	vec3 normal = vNormal * ( 1.0 - uUseNormalMap ) + normalize( transformColorToNormal( texCoord ) ) * uUseNormalMap;

	for( int i = 0; i < MAX_NUM_LIGHTS; ++i )
	{
		float directAttenuationFactor =	smoothstep( vLightLength[i], uLights[i].attenuation.start, uLights[i].attenuation.end );
			
		vec3 lightDirTBNNormalized = normalize( vLightDirTBN[i] );
		vec3 lightDirFromVertexTBNNormalized = normalize( vLightDirFromVertexTBN[i] );

		float apertureAngleDot = dot( lightDirTBNNormalized, -lightDirFromVertexTBNNormalized );
	
		float apertureAttenuationFactor = smoothstep( apertureAngleDot, uLights[i].aperture.outerConeDot, uLights[i].aperture.innerConeDot );
			
		totalDirect += getDirectLight( uLights[i], directAttenuationFactor * apertureAttenuationFactor, normal, lightDirFromVertexTBNNormalized, lightDirTBNNormalized, texCoord );
	}

	vec4 stdDirectLight = clamp( totalDirect, vec4( 0.0 ), vec4( 1, 1, 1, 1 ) );
	vec4 excessiveDirectLight = totalDirect - stdDirectLight;
	excessiveDirectLight *= .3;
	vec4 actualDirectLight =  stdDirectLight + excessiveDirectLight;

	vec4 tempFragColor = vec4( 0, 0, 0, 1.0 );

	tempFragColor += actualDirectLight;

	tempFragColor = texture( uDiffuseMap, texCoord ) * dot( -uLights[0].direction, vNormal );	
	tempFragColor += texture( uEmissiveMap, texCoord )*uUseEmissiveMap;
	tempFragColor.a = 1.0;


	return tempFragColor;
}

void main()
{
	vec2 texCoord = vec2( vTexCoord0.x, vTexCoord0.y );

	fragColor = getLighting( texCoord ) * ( 1.0 - uUnlit ) + texture( uDiffuseMap, texCoord ) * uUnlit;
	fragColor.a = 1.0;
	//fragColor = vColor;
}