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

in vec3 aPosition;
in vec3 aNormal;
in vec4 aColor;
in vec2 aTexCoord0;
in vec3 aTangent;
in vec3 aBitangent;

out vec4 vColor;
out vec2 vTexCoord;
out vec3 vModelPosition;

void main()
{
	vColor = aColor;
	vTexCoord = aTexCoord0;
	vModelPosition = aPosition.xyz;
	gl_Position = uMVPMatrix * vec4( aPosition, 1.0 );
}