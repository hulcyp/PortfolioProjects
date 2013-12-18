#version 330

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;
uniform sampler2D uTexels;
uniform float uFontHeight;

in vec4 myColor;
in vec2 texCoord;

out vec4 fragColor;

void main()
{
	fragColor = myColor*texture2D( uTexels, texCoord );
}