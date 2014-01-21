#include "ImporterApp.h"
#include <MCCamera.h>

using namespace pdh;

const double DELTA_TIME = 1/60.0;
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const float FOV = 45.0f;

int main()
{
	ImporterApp app(	DELTA_TIME,
						WINDOW_WIDTH,
						WINDOW_HEIGHT,
						FOV,
						"Importer App" );
	Camera* camera = new MCCamera( "Main Camera", vec3f( 0.0f, 0.0f, 100.0f ),
		vec2i( WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 ), mat4f::perspective( FOV, (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 100000.0f ) );

	app.initializeCamera( camera );
	app.startApp();

	return 0;
}