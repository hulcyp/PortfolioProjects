#pragma once
#include <GameApp.h>
#include <EventSystemInterface.h>
#include <AudioSystem.h>
#include <SoundCue.h>
#include  "MapLoader.h"
#include "ReverbArea.h"

namespace Monky
{
	class Clock;
	class DSP;
	class Mesh;
	class ASGeometry;

	class App : public GameApp
	{
	public:
		App( double deltaTime, int width, int height, float fov,  const char* windowTitle );
		virtual ~App();

		virtual bool onKeyDown( MonkyKey keyCode, int x, int y );
		
		void printMsgEvt( NamedProperties& params );
		
	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();

		virtual void renderDebugHUD();

	private:
		Clock* m_childClock;
		SoundCue* m_soundCue;
		MapLoader m_mapLoader;
		bool m_renderSoundCueDebug;
		bool m_renderReverbAreaDebug;
		float m_currentAngle;
	};
}