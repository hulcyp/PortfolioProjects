#include "RTSGame.h"
#include <MCCamera.h>
#include "RTSCamera.h"

using namespace pdh;

const double DELTA_TIME = 1/60.0;
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const float FOV = 45.0f;


int main()
{
	RTSGame app(	DELTA_TIME,
					WINDOW_WIDTH,
					WINDOW_HEIGHT,
					FOV,
					"RTS" );

	Camera* camera = new RTSCamera( "Main Camera", vec3f( 0.0f, 0.0f, 1.0f ), mat4f::ortho( 0.0f, (float)WINDOW_WIDTH*0.05f, (float)WINDOW_HEIGHT*0.05f, 0.0f, 1.0f, -1.0f ) );
	app.initializeCamera( camera );
	app.startApp();

	return 0;
}