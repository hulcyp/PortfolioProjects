#version 330

uniform float time;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;

uniform sampler2D uColorTexture;	// Blurred glow map
uniform sampler2D uDepthTexture;
uniform float uAspectRatio;
uniform int uScreenWidth;
uniform int uScreenHeight;

uniform sampler2D uBufferPassTextureColor0;	// Original scene
uniform sampler2D uBufferPassTextureColor1;	// Glow map

in vec2 vTexCoord;

out vec4 fragColor;

float pixelWidth = 1.0 / uScreenWidth;
float pixelHeight = 1.0 / uScreenHeight;

void main()
{
	vec4 dst = texture( uBufferPassTextureColor0, vTexCoord );
	vec4 src = texture( uColorTexture, vTexCoord );
	
	fragColor = clamp( ( src + dst ) - ( src * dst ), 0.0, 1.0 );
	fragColor.a = 1.0;
}