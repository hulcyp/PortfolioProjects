uniform float time;
varying vec4 myColor;
void main()
{
	gl_FragColor = myColor * vec4( sin( time ), 1, 1, 1 );
}