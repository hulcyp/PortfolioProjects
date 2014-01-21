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


//debugging
uniform int unlit;

layout(location=0)in vec3 aPosition;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec4 aColor;
layout(location=3)in vec2 aTexCoord0;
layout(location=4)in vec3 aTangent;
layout(location=5)in vec3 aBitangent;

out vec4 vColor;
out vec2 vTexCoord0;
out vec4 vScreenPos;
out vec3 vCameraToVertex;
out vec3 vWorldPosition;
out vec3 vNormal;

out vec3 vLightDirFromVertexTBN[ MAX_NUM_LIGHTS ];
out float vLightLength[ MAX_NUM_LIGHTS ];
out vec3 vLightDirTBN[ MAX_NUM_LIGHTS ];

void orthoNormalizeTB( out vec3 oTangent, out vec3 oBitangent )
{
	oTangent = aTangent - dot( aNormal, aTangent )*aNormal;
	oBitangent = aBitangent - dot( aNormal, aBitangent )*aNormal - 
	( ( dot( oTangent, aBitangent ) * oTangent ) / dot( aTangent, aTangent ) );
	
	oTangent = normalize( oTangent );
	oBitangent = normalize( oBitangent );
}

mat4 getTBNMatrix()
{
	vec3 orthoTangent = aTangent;
	vec3 orthoBitangent = aBitangent;
	orthoNormalizeTB( orthoTangent, orthoBitangent );
	return mat4( vec4( orthoTangent, 0.0 ), vec4( orthoBitangent, 0.0 ), vec4( aNormal, 0.0 ), vec4( 0.0, 0.0, 0.0, 1.0 ) );
}

void main()
{	
	mat4 inverseTBN = inverse( getTBNMatrix() );
	mat4 invModel = inverse( uModelMatrix );
	
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
	vCameraToVertex = normalize( aPosition - cameraPosInModel.xyz ).xyz;
	//vCameraToVertex = normalize( inverseTBN * vec4( vCameraToVertex, 0.0 ) ).xyz;
		
	vColor = vec4( aColor.r, aColor.g, aColor.b, 1.0 );
	vTexCoord0 = aTexCoord0;
	gl_Position = uMVPMatrix * vec4( aPosition, 1.0 );
	vScreenPos = gl_Position;
	vWorldPosition = aPosition;
	vNormal = aNormal;
}