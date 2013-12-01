#pragma once
#include <GlutApp.h>
#include "ExplosionParticleSystem.h"
#include "FireworksParticleSystem.h"

namespace pdh
{
	class GameWorld : public GlutApp
	{
	public:
		GameWorld( double deltaTime, int width, int height, float fov, const char* windowTitle );
		virtual ~GameWorld();
		
		virtual void updateFrame();
		virtual void onKeyDown( unsigned char keyCode, int x, int y );
		virtual void onKeyUp( unsigned char keyCode, int x, int y );
		virtual void onSpecialKeyDown( int keyCode, int x, int y );
		virtual void onMouseMove( int mouseX, int mouseY );
	
	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();

	protected:
		const double DELTA_TIME;
	};
}