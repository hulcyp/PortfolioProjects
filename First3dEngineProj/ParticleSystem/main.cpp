#include "PSGameWorld.h"
#include <Renderer.h>
#include "MCCamera.h"

using namespace pdh;

const double DELTA_TIME = 1/60.0;
const int INITIAL_SCREEN_WIDTH = 800;
const int INITIAL_SCREEN_HEIGHT = 600;

int main()
{
	PSGameWorld psgameWorld(	DELTA_TIME,
								INITIAL_SCREEN_WIDTH,
								INITIAL_SCREEN_HEIGHT,
								45.0f,
								"Particle System Test" );
	
	psgameWorld.initializeCamera( 
		new MCCamera(	vec3( 0.0f, 0.0f, 100.0f ), 
									vec2i(	(int)( INITIAL_SCREEN_WIDTH * .5 ),
											(int)( INITIAL_SCREEN_HEIGHT * .5 ) ) ) );


	psgameWorld.startApp();

	return 0;
}