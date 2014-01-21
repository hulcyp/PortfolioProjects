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
uniform sampler2D uEmissiveMap;
uniform int uUseEmissiveMap;

//debugging
uniform int uUnlit;

in vec4 vColor;
in vec2 vTexCoord0;
in vec3 vNormal;

out vec4 fragColor;


vec4 getLighting( in vec2 texCoord )
{
	vec4 tempFragColor;

	tempFragColor = texture( uDiffuseMap, texCoord ) * dot( -uLights[0].direction, vNormal );	
	tempFragColor += texture( uEmissiveMap, texCoord )*uUseEmissiveMap;
	tempFragColor.a = 1.0;

	return tempFragColor;
}

void main()
{
	vec2 texCoord = vec2( vTexCoord0.x, vTexCoord0.y );

	fragColor = getLighting( texCoord ) * ( 1.0 - uUnlit ) + texture( uDiffuseMap, texCoord ) * uUnlit;
	fragColor = vColor;
}