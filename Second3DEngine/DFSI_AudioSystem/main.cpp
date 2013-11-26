#include "App.h"
#include <MCCamera.h>
#include <FileManager.h>

using namespace Monky;

const double DELTA_TIME = 1/60.0;
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const float FOV = 45.0f;



int main( int argc, char** argv )
{
	getFileManager().initialize( "./data/", "data.zip", FileManager::DATA_FOLDER_THEN_ARCHIVE, "" );
		

	App app(	DELTA_TIME,
					WINDOW_WIDTH,
					WINDOW_HEIGHT,
					FOV,
					"Optimization" );

	Camera* camera = new MCCamera( "Main Camera", vec3f( 0.0f, 0.0f, 1.0f ),
									vec2i( WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 ),
									mat4f::perspective( FOV, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100000.0f ) );
	app.initializeCamera( camera );
	app.runApp();

	return 0;
}