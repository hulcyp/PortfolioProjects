#include "App.h"
#include <Renderer.h>
#include <Clock.h>
#include <MCCamera.h>
#include <MeshFactory.h>
#include "CustomDSP.h"

namespace Monky
{
	App::App( double deltaTime, int width, int height, float fov,  const char* windowTitle )
		:	GameApp( deltaTime, width, height, fov, windowTitle )
		,	m_mapLoader( "xml/AudioTestMap.xml", this )
		,	m_renderSoundCueDebug( true )
		,	m_renderReverbAreaDebug( true )
		,	m_currentAngle( 0.0f )
	{		
		Sound::loadSoundsFromXMLFile( "sounds/", "Sounds.xml" );
		SoundCue::loadSoundCuesFromXMLFile( "sounds/", "SoundCues.xml" );
		ReverbArea::loadReverbAreasFromXMLFile( "sounds/", "ReverbAreas.xml" );
		m_soundCue = SoundCue::getSoundCue( "DarkHorses" );
		m_mapLoader.loadMap();
		m_soundCue->play();
	}
	//-------------------------------------------------------
	App::~App()
	{}
	//-------------------------------------------------------
	bool App::onKeyDown( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		used = GameApp::onKeyDown( keyCode, x, y );
		if( !used )
		{
			switch( keyCode )
			{
			case MONKY_p:
				if( !m_soundCue->isPaused() )
					m_soundCue->pause();
				else
					m_soundCue->unpause();
				used = true;
				break;
			case MONKY_SPACE:
				m_soundCue->play();
				used = true;
				break;
			case MONKY_v:
				m_soundCue->stop();
				used = true;
				break;
			case MONKY_y:
				AudioSystem::getInstance()->playSound( "DarkHorses2D" );
				used = true;
				break;
			case MONKY_F9:
				SoundCue::cleanupSoundCues();
				SoundCue::loadSoundCuesFromXMLFile( "sounds/", "SoundCues.xml" );
				used = true;
				break;
			case MONKY_8:
				m_renderSoundCueDebug = !m_renderSoundCueDebug;
				used = true;
				break;
			case MONKY_9:
				m_renderReverbAreaDebug = !m_renderReverbAreaDebug;
				used = true;
				break;
			case MONKY_EQUALS:
				m_soundCue->setVolume( m_soundCue->getVolume() + 0.1f );
				used = true;
				break;
			case MONKY_MINUS:
				m_soundCue->setVolume( m_soundCue->getVolume() - 0.1f );
				used = true;
				break;
			}
		}
		return used;
	}
	//-------------------------------------------------------
	void App::printMsgEvt( NamedProperties& params )
	{
		std::string msg;
		if( params.get( "msg", msg ) == NamedProperties::SUCCESS )
			consolePrintf( msg.c_str() );
	}
	//-------------------------------------------------------
	void App::updateSimulation()
	{
		GameApp::updateSimulation();

		AudioSystem::getInstance()->setListenerAttributes( m_camera->getPosition(), vec3f(), m_camera->getDirection(), m_camera->getUpVector() );
		AudioSystem::getInstance()->update();
		m_currentAngle += m_frameClock.getDeltaSecondsFloat() * 20.0f;
		SoundCue* drumLoop = SoundCue::getSoundCue( "DarkHorses03" );
		if( drumLoop != nullptr )
		{
			mat3f rotation( mat3f::IDENTITY );
			rotation.rotate( 0.0f, m_currentAngle, 0.0f );
			drumLoop ->set3DConeOrientation(  rotation );
		}

	}
	//-------------------------------------------------------
	void App::updateDisplay()
	{		
		GameApp::updateDisplay();

		NamedProperties params;
		params.set( "length", 10.0f );
		fireEvent( "renderAxis", params );

		GameApp::renderHUD();
		GameApp::renderConsole();
		if( m_renderSoundCueDebug )
			SoundCue::renderSoundCueDebugInfo( m_camera );
		if( m_renderReverbAreaDebug )
			ReverbArea::renderAllReverbAreaDebugInfo( m_camera );
		renderDebugHUD();
	}
	//-------------------------------------------------------
	void App::renderDebugHUD()
	{
		GameApp::renderDebugHUD();
		if( m_renderDebugHUD )
		{
		}
	}
}
