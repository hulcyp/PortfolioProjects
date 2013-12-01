#include "LightingWorld.h"
#include <MCCamera.h>

using namespace pdh;

const double DELTA_TIME = 1/60.0;
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const float FOV = 45.0f;

int main()
{
	LightingWorld lightingWorld(	DELTA_TIME,
									WINDOW_WIDTH,
									WINDOW_HEIGHT,
									FOV,
									"Lighting World" );
	Camera* camera = new MCCamera( vec3f( 0.0f, 0.0f, 20.0f ),
									vec2i( WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 ) );

	lightingWorld.initializeCamera( camera );
	lightingWorld.startApp();

	return 0;									
}