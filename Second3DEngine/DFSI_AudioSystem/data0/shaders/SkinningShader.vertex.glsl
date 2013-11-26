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

const int MAX_NUM_BONES = 100;

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;

uniform sampler2D uDiffuseMap;
uniform sampler2D uNormalMap;
uniform sampler2D uEmissiveMap;
uniform int uUseNormalMap;
uniform int uUseEmissiveMap;

uniform mat4 Bones[MAX_NUM_BONES];
uniform Light uLights[ MAX_NUM_LIGHTS ];

layout(location=0)in vec3 aPosition;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec4 aColor;
layout(location=3)in vec2 aTexCoord0;
layout(location=4)in vec3 aTangent;
layout(location=5)in vec3 aBitangent;
layout(location=6)in vec4 aBoneIndices;
layout(location=7)in vec3 aWeights;

out vec4 vColor;
out vec2 vTexCoord0;
out vec3 vNormal;

void main()
{
	float weight4 = 1.0 - (aWeights.x + aWeights.y + aWeights.z);
	vec4 vertPosition = uModelMatrix * vec4( aPosition, 1.0 );
	vec4 tempPosition;

	vec4 weights = vec4( aWeights, weight4 );

	for( int i = 0; i < 4; ++i )
	{
		int index = int( aBoneIndices[i] );
		tempPosition += Bones[ index ] * vertPosition * weights[i];
	}
	
	gl_Position =  uProjectionMatrix * uViewMatrix * tempPosition;
	//gl_Position = uMVPMatrix * vertPosition;

	//vColor = vec4( aBoneIndices[0]/38, 0.0, 0.0, 1.0 );//aColor;
	float r = aBoneIndices[0] / 38.0f;
	float g = 1 - r;
	vColor = vec4( r, g, 0.0f, 1.0f );
	//vColor = vec4( weights[2], 0.0, 0.0, 1.0 );
	vTexCoord0 = aTexCoord0;
	vNormal = aNormal;
}