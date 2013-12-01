#pragma once
#include <GameWorld.h>
#include "FountainParticleSystem.h"
#include "SmokeParticleSystem.h"
#include "RingParticleSystem.h"
#include "FireworksParticleSystem.h"


namespace pdh
{
	class Font;

	class PSGameWorld : public GameWorld
	{
	public:
		PSGameWorld( double deltaTime, int width, int height, float fov, const char* windowTitle );
		virtual ~PSGameWorld();


	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();
		virtual void renderHUD();

	protected:
		std::shared_ptr< ExplosionParticleSystem > m_explosionPS;
		std::shared_ptr< FireworksParticleSystem > m_fireworksPS;
		std::shared_ptr< FountainParticleSystem > m_fountainPS;
		std::shared_ptr< SmokeParticleSystem > m_smokePS;
		std::shared_ptr< RingParticleSystem > m_ringPS;
		Font* m_font;
		int m_numParticles;
	};
}