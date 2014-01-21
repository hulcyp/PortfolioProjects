#include "ImporterApp.h"
#include <Mesh.h>
#include <Renderer.h>
#include <Material.h>
#include <CommonEngineIncludes.h>
#include <Model.h>
#include <ShaderProgram.h>
#include <Font.h>
#include <P3DImporter.h>
#include <MathFuncs.h>
#include <Scene.h>
#include <GDOManager.h>

namespace pdh
{
	ImporterApp::ImporterApp( double deltaTime, int width, int height, float fov,  const char* windowTitle )
		:	GameWorld( deltaTime, width, height, fov, windowTitle )
		,	m_currentScene( 0 )
		,	m_enableLighting( true )
		,	m_pauseAnimations( false )
		,	m_freeCameraOn( false )
	{
		m_light = new Material::Light();
		m_light->color = color::WHITE;
		m_light->color.a = 0.0f;
		m_light->attenuation.start = 25.0f;
		m_light->attenuation.end = 50.0f;
		m_light->aperture.innerConeDot = -1.0f;
		m_light->aperture.outerConeDot = -1.0f;
		m_light->direction = vec3f( -1.0f, -1.0f, -1.0f );
		m_light->position = vec3f( 50.0f, 75.0f, 50.0f );
	
		importMeshes();

		registerConsoleCommand( "event", "Fire event. Usage \"event [eventName]\"", 
			[&]( const std::string& args ) 
			{
				fireEvent( args ); 
			} );
	}

	ImporterApp::~ImporterApp()
	{}

	bool ImporterApp::onKeyDown( unsigned char keyCode, int x, int y )
	{
		bool used = false;
		used = GameWorld::onKeyDown( keyCode, x, y );
		if( !used )
		{
			keyCode = toupper( keyCode );
			switch( keyCode )
			{
			case 'L':
				m_enableLighting = !m_enableLighting;
				m_renderer->enableLighting( m_enableLighting );
				break;
			case 'P':
				m_pauseAnimations = !m_pauseAnimations;
				if( m_pauseAnimations )
				{
					m_actorManager.pauseAllAnimations();
				}
				else
				{
					m_actorManager.playAllAnimations();
				}
			case 'C':
				m_freeCameraOn = !m_freeCameraOn;
				if( m_freeCameraOn )
					m_renderer->pushCamera( m_camera );
				else
					m_renderer->popCamera();
				break;
			}
		}
		return used;
	}

	bool ImporterApp::onSpecialKeyDown( int keyCode, int x, int y )
	{
		bool used = false;
		used = GameWorld::onSpecialKeyDown( keyCode, x, y );
		if( !used )
		{
			switch( keyCode )
			{
			case GLUT_KEY_PAGE_UP:
				switchScene( MathFuncs<int>::wrap( m_currentScene + 1, 0, m_scenes.size() - 1 ) );
				break;
			case GLUT_KEY_PAGE_DOWN:
				switchScene( MathFuncs<int>::wrap( m_currentScene - 1, 0, m_scenes.size() - 1 ) );
				break;
			}
		}

		return used;
	}

	void ImporterApp::updateSimulation()
	{
		GameWorld::updateSimulation();
		createArrowGDO( m_light->direction, 15.0f, m_light->position );
		m_actorManager.update( DELTA_TIME );
		
		if( m_scenes[ m_currentScene ]->cameras.size() > 0 )
			m_scenes[ m_currentScene ]->cameras[0]->update( DELTA_TIME );

	}

	void ImporterApp::updateDisplay()
	{	
		glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		matStackf& modelStack = m_renderer->getModelStack();
		modelStack.loadIdentity();
		if( m_camera && !m_freeMouse )
		{
			m_camera->updateInput();
			m_camera->update( DELTA_TIME );
		}
		m_renderer->setWireFrame( m_wireFrame );
		m_renderer->setProjectionMode( Renderer::PH_PERSPECTIVE );
		m_actorManager.renderActors( m_renderer );
		m_GDOManager->renderGDOs( m_renderer, m_time );
		m_renderer->renderAxis( 10.0f );
		m_renderer->setWireFrame( false );
		GameWorld::renderHUD();
		GameWorld::renderConsole();
		GameWorld::renderDebugHUD();

		glutSwapBuffers();
	}

	void ImporterApp::renderHUD()
	{
		matStackf& modelStack = m_renderer->getModelStack();
		m_renderer->setProjectionMode( Renderer::PH_ORTHOGRAPHIC );
		modelStack.push();
		modelStack.loadIdentity();
		m_font->renderText( "Lighting enabled: " + toString( m_enableLighting ), vec3f( 0.0f, (float)m_screenHeight ), 12.0f, color::GREEN, Font::TOPLEFT );
		m_font->renderText( "Current scene: " + m_scenes[m_currentScene]->name, vec3f( 0.0f, (float) ( m_screenHeight - 12.0f ) ), 24.0f, color::WHITE, Font::TOPLEFT );
		modelStack.pop();
		m_renderer->setProjectionMode( Renderer::PH_PERSPECTIVE );
	}

	void ImporterApp::importMeshes()
	{

		const char* file = "../data/meshes/Meshes.xml"; 
		tinyxml2::XMLDocument doc;
		doc.LoadFile( file );

		assertion( !doc.Error(), "Failed to load %s file", file );

		tinyxml2::XMLElement* root = doc.FirstChildElement( "Meshes" );
		tinyxml2::XMLElement* mesh;
		for( mesh = root->FirstChildElement( "Mesh" ); mesh != 0; mesh = mesh->NextSiblingElement( "Mesh" ) )
		{
			std::string fileName = mesh->GetText();
			P3DImporter importer( "../data/meshes/" + fileName );
			Scene* scene = new Scene( fileName );
			importer.importData( scene );

			for( size_t i = 0; i < scene->actors.size(); ++i )
			{
				scene->actors[i]->getMesh()->addLight( m_light, 0 );
			}
			m_scenes.push_back( scene );		
		}		
		
		for( size_t i = 0; i < m_scenes[0]->actors.size(); ++i )
		{			
			m_actorManager.addActor( m_scenes[0]->actors[i] );
		}
		if( m_scenes[0]->cameras.size() > 0 )
		{
			m_scenes[0]->cameras[0]->play();
			m_renderer->pushCamera( m_scenes[0]->cameras[0] );
		}
		m_actorManager.playAllAnimations();
	}

	void ImporterApp::switchScene( int newScene )
	{
		m_actorManager.stopAllAnimations();
		for( size_t i = 0; i < m_scenes[ m_currentScene ]->actors.size(); ++i )
		{
			m_actorManager.removeActor( m_scenes[ m_currentScene ]->actors[i]->getID() );
		}

		assertion( newScene < (int)m_scenes.size(), "Scene index: %d out of bounds", newScene );
		for( size_t i = 0; i < m_scenes[ newScene ]->actors.size(); ++i )
		{
			m_actorManager.addActor( m_scenes[ newScene ]->actors[i] );
		}
		m_actorManager.playAllAnimations();

		if( m_scenes[ m_currentScene ]->cameras.size() > 0 )
		{
			m_renderer->pushCamera( m_scenes[m_currentScene]->cameras[0] );
			m_scenes[m_currentScene]->cameras[0]->stop();
			m_renderer->popCamera();		
		}

		if( m_scenes[ newScene ]->cameras.size() > 0 )
		{
			m_renderer->pushCamera( m_scenes[newScene]->cameras[0] );
			m_scenes[newScene]->cameras[0]->play();
		}
		else
		{
			m_renderer->pushCamera( m_camera );
		}
		m_currentScene = newScene;		
	}
}