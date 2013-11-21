uniform float time;
varying vec4 myColor;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex + 5*vec4( 0, gl_Vertex.y*sin( time + gl_Vertex.x*.01 ), 0, 0 );	
	myColor = gl_Color;
}