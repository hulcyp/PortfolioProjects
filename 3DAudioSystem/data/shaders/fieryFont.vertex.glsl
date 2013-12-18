#version 330

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;
uniform sampler2D uTexels;
uniform float uFontHeight;
uniform sampler2D uNoiseTexture;

in vec3 aPosition;
in vec3 aNormal;
in vec4 aColor;
in vec2 aTexCoord0;
in vec3 aTangent;
in vec3 aBitangent;

out vec4 vColor;
out vec2 vTexCoord;
out vec3 vModelPosition;
out float vNormalizedHeight;

void main()
{
	vModelPosition = aPosition;
	vNormalizedHeight = aPosition.y / uFontHeight;
	gl_Position = uMVPMatrix * vec4( aPosition, 1 );
	vColor = aColor;
	vTexCoord = aTexCoord0;
}