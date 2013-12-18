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

uniform sampler2D uDiffuseMap;
uniform sampler2D uNormalMap;
uniform sampler2D uSpecularMap;
uniform sampler2D uEmissiveMap;

uniform Light uLights[ MAX_NUM_LIGHTS ];

uniform vec3 lightPos;
uniform vec3 cameraPos;
//debugging
uniform int unlit;
uniform int renderTangentSpace;
uniform int enableDiffuse;
uniform int enableSpecular;
uniform int enableFog;
uniform int enableEmissive;
uniform int enableParallax;

in vec4 color;
in vec2 texCoord0;
in vec3 lightDir;
in vec4 screenPos;
in vec3 cameraToVertex;
in vec3 worldPosition;

in vec3 vLightDirFromVertexTBN[ MAX_NUM_LIGHTS ];
in float vLightLength[ MAX_NUM_LIGHTS ];
in vec3 vLightDirTBN[ MAX_NUM_LIGHTS ];

out vec4 fragColor;

float FOG_SCALE = 10000.0;
vec4 FOG_COLOR = vec4( 0.0, 0.0, 0.0, 1.0 );
float SPECULAR_POWER = 32.0;

vec3 transformColorToNormal( in vec2 texCoord )
{	
	vec4 col = texture( uNormalMap, texCoord );
	return 2.0*( col.rgb ) - vec3( 1.0 );
}

float getSpecularFactor( vec3 normal )
{	
	vec3 idealLight = reflect( normalize( cameraToVertex ),  normal );
	
	float specularPower;
	for( int i = 0; i < 1; ++i )
	{
		specularPower += pow( dot( idealLight, normalize( vLightDirFromVertexTBN[i] ) ), SPECULAR_POWER )*.5 * uLights[i].color.a;
	}
	return specularPower;
}

vec3 calcRGBEncodingForXYZ( vec3 xyz )
{
	return xyz * .5 + vec3( .5 );
}

void getFogInterpFactors( out float fogFactor, out float colorFactor )
{	
	float fogIntensity = screenPos.z / FOG_SCALE;
	fogFactor = clamp( fogIntensity, 0.0, 1.0 );
	colorFactor = 1.0 - fogFactor;
}

vec4 getDirectLight( in Light light, in float attenuationFactor, in vec3 normal, in vec3 lightDirFromVertexTBNNormalized, in vec3 lightDirTBNNormalized, in vec2 texCoord )
{
	float unAttenuated = max( 0.0, dot( normal, lightDirFromVertexTBNNormalized ) );
	float directIntensity = max( 0.0, unAttenuated * attenuationFactor ) * ( light.color.a ) + max( 0.0, dot( normal, -lightDirTBNNormalized ) ) * ( 1.0 - light.color.a );
	return vec4( texture( uDiffuseMap, texCoord ).rgb * directIntensity * light.color.rgb, 1.0 );
}

vec4 getSpecularLight( in Light light, float attenuationFactor, in vec3 normal, in vec3 lightDirFromVertexTBNNormalized, in vec3 lightDirTBNNormalized, in vec2 texCoord )
{	
	vec3 idealLight = reflect( normalize( cameraToVertex ), normal );
	
	float unAttenuatedSpecPower = pow( dot( idealLight, lightDirFromVertexTBNNormalized ), SPECULAR_POWER );
	float directionalLight = pow( dot( idealLight, -lightDirTBNNormalized ), SPECULAR_POWER );
	float specularPower = unAttenuatedSpecPower * attenuationFactor * light.color.a +  directionalLight * ( 1.0 - light.color.a );
	
	float specularIntensity = max( 0.0, specularPower );
	return specularIntensity*texture( uSpecularMap, texCoord );
}

vec4 getADSLighting( in vec2 texCoord )
{
	vec4 totalDirect;
	vec4 totalSpecular;
	vec4 totalAmbient = vec4( 0, 0, 0, 1.0 );
	vec3 normal = normalize( transformColorToNormal( texCoord ) );
	for( int i = 0; i < MAX_NUM_LIGHTS; ++i )
	{
		float normalizedDistance = ( vLightLength[i] - uLights[i].attenuation.start ) / ( uLights[i].attenuation.end - uLights[i].attenuation.start );
		float constantAttenFactor = 1.0;
		if( normalizedDistance > uLights[i].attenuation.end )
			constantAttenFactor = 0.0;
		float directAttenuationFactor = dot( vec4( constantAttenFactor, ( 1 - normalizedDistance ), 
											( 1 / ( 1.0 + normalizedDistance ) ), 
											( 1 / ( 1.0 + normalizedDistance * normalizedDistance ) -0.5 ) ), uLights[i].attenuationType );
		
		vec3 lightDirTBNNormalized = normalize( vLightDirTBN[i] );
		vec3 lightDirFromVertexTBNNormalized = normalize( vLightDirFromVertexTBN[i] );

		float apertureAngleDot = dot( lightDirTBNNormalized, -lightDirFromVertexTBNNormalized );
		float normalizedDot = ( uLights[i].aperture.innerConeDot - apertureAngleDot ) / ( uLights[i].aperture.innerConeDot - uLights[i].aperture.outerConeDot );
		normalizedDot = clamp( normalizedDot, 0.0, 1.0 );

		float constantAperFactor = 1.0;
		if( normalizedDot > uLights[i].aperture.outerConeDot )
			constantAperFactor = 0.0;
		float apertureAttenuationFactor = dot( vec4( constantAperFactor, ( 1 - normalizedDot ), ( 1 /( 1.0 + normalizedDot ) - 0.5 ), ( 1 / ( 1.0 + normalizedDot * normalizedDot ) ) - 0.5 ), uLights[i].attenuationType );
			
		totalDirect += getDirectLight( uLights[i], directAttenuationFactor * apertureAttenuationFactor, normal, lightDirFromVertexTBNNormalized, lightDirTBNNormalized, texCoord );
		totalSpecular += getSpecularLight( uLights[i], directAttenuationFactor * apertureAttenuationFactor, normal, lightDirFromVertexTBNNormalized, lightDirTBNNormalized, texCoord );
		totalAmbient += uLights[i].color * uLights[i].ambientFactor;
	}

	vec4 stdDirectLight = clamp( totalDirect, vec4( 0.0 ), vec4( 1, 1, 1, 1 ) );
	vec4 excessiveDirectLight = totalDirect - stdDirectLight;
	excessiveDirectLight *= .3;
	vec4 actualDirectLight =  stdDirectLight + excessiveDirectLight;

	vec4 tempFragColor = vec4( 0, 0, 0, 1.0 );

	tempFragColor += actualDirectLight;
	//tempFragColor += totalSpecular;
	//tempFragColor += totalAmbient;

	return tempFragColor;	
}

vec4 getColorFromLighting( in vec2 texCoord )
{
	vec3 normal = transformColorToNormal( texCoord );
	float diffuseIntensity = max(0.0, dot( normalize( normal ), normalize( lightDir ) ) );
	
	float specularIntensity = max( 0.0, getSpecularFactor( normalize( normal ) ) );
	vec4 specularColor = vec4( 0.0, 1.0, 0.0, 1.0 );
	vec4 tempFragColor = vec4( 0.0, 0.0, 0.0, 1.0 );
	
		
	tempFragColor = getADSLighting( texCoord );
	
	tempFragColor += texture( uEmissiveMap, texCoord );

	float fogFactor;
	float colorFactor;
	getFogInterpFactors( fogFactor, colorFactor );
	tempFragColor *= colorFactor;
	tempFragColor += fogFactor*FOG_COLOR;
	
	return tempFragColor;
}

void main()
{	
	vec2 texCoord = vec2( texCoord0.x, 1.0 - texCoord0.y );
	
	vec4 tempFragColor;

	tempFragColor = texture( uDiffuseMap, texCoord ) * unlit;

	tempFragColor = getColorFromLighting( texCoord ) * ( 1 - unlit );

	fragColor = tempFragColor;
	//fragColor = texture( uSpecularMap, texCoord );
	//fragColor = texture( uDiffuseMap, texCoord );
	//fragColor = texture( uNormalMap, texCoord );
	//fragColor = texture( uDiffuseMap, texCoord );
	//fragColor = vec4( texCoord.x, texCoord.y, 0.0, 1.0 );
	//fragColor = vec4( vLightDirFromVertexTBN[0], 1.0 );
	//fragColor = uLights[0].color;
	//fragColor = vec4( uLights[0].attenuation.start, uLights[0].attenuation.end, 0.0, 1.0 );
}

