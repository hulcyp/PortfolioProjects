#version 330

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;

layout(location=0)in vec3 aPosition;
layout(location=1)in vec4 aColor;
layout(location=2)in vec2 aTexCoord0;

out vec4 vColor;
out vec2 vTexCoord0;

void main()
{
	gl_Position = uMVPMatrix * vec4( aPosition, 1.0 );
	vColor = aColor;
	vTexCoord0 = aTexCoord0;
}