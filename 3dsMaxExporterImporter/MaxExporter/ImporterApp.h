#pragma once
#include <GameWorld.h>
#include <Material.h>

namespace pdh
{
	class Scene;

	class ImporterApp : public GameWorld
	{
	public:


		ImporterApp( double deltaTime, int width, int height, float fov,  const char* windowTitle );
		virtual ~ImporterApp();

		virtual bool onKeyDown( unsigned char keyCode, int x, int y );
		virtual bool onSpecialKeyDown( int keyCode, int x, int y );
				

	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();
		virtual void renderHUD();

		void importMeshes();
		void switchScene( int newScene );

	private:
		std::vector< Scene* > m_scenes;
		int m_currentScene;
		Material::Light* m_light;
		bool m_enableLighting;
		bool m_pauseAnimations;
		bool m_freeCameraOn;
		Camera* m_defaultCamera;
	};
}