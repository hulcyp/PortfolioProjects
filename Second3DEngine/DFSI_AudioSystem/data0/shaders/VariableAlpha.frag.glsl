#version 330

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;

uniform sampler2D uDiffuseMap;
uniform int uUseDiffuseMap;
uniform float uAlpha;

in vec4 vColor;
in vec2 vTexCoord0;

out vec4 fragColor;

void main()
{
	fragColor = vColor * texture( uDiffuseMap, vTexCoord0 ) * uUseDiffuseMap + vColor * ( 1 - uUseDiffuseMap );
	fragColor.a = uAlpha;
}