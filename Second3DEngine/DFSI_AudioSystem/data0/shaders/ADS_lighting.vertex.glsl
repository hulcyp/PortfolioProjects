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

layout(location=0)in vec3 aPosition;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec4 aColor;
layout(location=3)in vec2 aTexCoord0;
layout(location=4)in vec3 aTangent;
layout(location=5)in vec3 aBitangent;

out vec4 color;
out vec2 texCoord0;
out vec3 lightDir;
out vec4 screenPos;
out vec3 cameraToVertex;
out vec3 worldPosition;

out vec3 vLightDirFromVertexTBN[ MAX_NUM_LIGHTS ];
out float vLightLength[ MAX_NUM_LIGHTS ];
out vec3 vLightDirTBN[ MAX_NUM_LIGHTS ];

void orthoNormalizeTB( out vec3 oTangent, out vec3 oBitangent )
{
	oTangent = aTangent - dot( aNormal, aTangent )*aNormal;
	oBitangent = -aBitangent - dot( aNormal, -aBitangent )*aNormal - 
	( ( dot( oTangent, -aBitangent ) * oTangent ) / dot( aTangent, aTangent ) );
	
	oTangent = normalize( oTangent );
	oBitangent = normalize( oBitangent );
}

mat4 getTBNMatrix()
{
	vec3 orthoTangent;
	vec3 orthoBitangent;
	orthoNormalizeTB( orthoTangent, orthoBitangent );
	return mat4( vec4( orthoTangent, 0.0 ), vec4( orthoBitangent, 0.0 ), vec4( aNormal, 0.0 ), vec4( 0.0, 0.0, 0.0, 1.0 ) );
}

void main()
{	
	mat4 inverseTBN = inverse( getTBNMatrix() );
	mat4 invModel = inverse( uModelMatrix );

	
	vec4 lightPosInModel = invModel * vec4( lightPos, 1.0 );
	lightDir = normalize( lightPosInModel.xyz - aPosition ).xyz;
	lightDir = ( inverseTBN * vec4( lightDir, 1.0 ) ).xyz;
	

	for( int i = 0; i < MAX_NUM_LIGHTS; ++i )
	{
		vec4 lightPosInModel = invModel * vec4( uLights[i].position, 1.0 );
		vec3 lightDirFromVertexModelSpace = normalize( lightPosInModel.xyz - aPosition );
		vec3 lightDirFromVertexTBNSpace = ( inverseTBN * vec4( lightDirFromVertexModelSpace, 0.0 ) ).xyz; 
		vLightDirFromVertexTBN[i] = lightDirFromVertexTBNSpace;

		vec4 lightDirTBN = ( inverseTBN * invModel * vec4( normalize( uLights[i].direction ), 0.0 ) );
		vLightDirTBN[i] = lightDirTBN.xyz;

		vLightLength[i] = length( lightPosInModel.xyz - aPosition.xyz );
	}

	vec4 cameraPosInModel = invModel * vec4( cameraPos, 1.0 );
	cameraToVertex = normalize( aPosition - cameraPosInModel.xyz ).xyz;
	cameraToVertex = normalize( inverseTBN * vec4( cameraToVertex, 0.0 ) ).xyz;
		
	color = vec4( aColor.r, aColor.g, aColor.b, 1.0 );
	texCoord0 = aTexCoord0;
	gl_Position = uMVPMatrix * vec4( aPosition, 1.0 );
	screenPos = gl_Position;
	worldPosition = aPosition;	
}