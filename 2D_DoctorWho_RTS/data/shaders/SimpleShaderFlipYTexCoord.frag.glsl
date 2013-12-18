#version 330

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;
uniform float uFontHeight;

uniform sampler2D uDiffuseMap;
uniform int uUseDiffuseMap;

in vec4 vColor;
in vec2 vTexCoord0;

out vec4 fragColor;

void main()
{
	vec2 texCoord = vec2( vTexCoord0.x, 1.0 - vTexCoord0.y );
	fragColor = vColor * texture( uDiffuseMap, texCoord ) * uUseDiffuseMap + vColor * ( 1 - uUseDiffuseMap );
}