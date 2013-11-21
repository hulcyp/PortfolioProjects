#pragma once
#include <list>

namespace Monky
{
	class GraphicDebugObject;
	class ActorManager;
	class Renderer;

	class GDOManager
	{
	public:
		GDOManager( ActorManager* aManager );
		~GDOManager();

		ActorManager* getActorManager() { return m_actorManager; }
		
		void addGDO( GraphicDebugObject* gdo );
		void renderGDOs( Renderer* renderer, float currentTime );
		float getCurrentTime() { return m_currentTime; }
		void clearAllGDOs();
		

	private:
		std::list< GraphicDebugObject* > m_GDOs;
		ActorManager* m_actorManager;
		float m_currentTime;
	};
}