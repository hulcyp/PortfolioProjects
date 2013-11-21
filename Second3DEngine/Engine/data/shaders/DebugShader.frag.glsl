#version 330

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;

uniform float uAlpha;
uniform vec3 uColor;

in vec4 vColor;

out vec4 fragColor;

void main()
{
	fragColor = vColor * vec4( uColor, uAlpha );
}