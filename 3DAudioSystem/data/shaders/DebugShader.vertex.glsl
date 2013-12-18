#version 330

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;

uniform float uAlpha;
uniform vec3 uColor;

in vec3 aPosition;
in vec3 aNormal;
in vec4 aColor;
in vec2 aTexCoord0;
in vec3 aTangent;
in vec3 aBitangent;

out vec4 vColor;

void main()
{
	vColor = aColor;
	gl_Position = uMVPMatrix * vec4( aPosition, 1 );
}