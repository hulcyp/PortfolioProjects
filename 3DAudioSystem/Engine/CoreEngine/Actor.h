#pragma once
#include <Vector3.h>
#include "CommonEngineIncludes.h"
#include "Object.h"

#define DEFAULT_ACTOR_TYPE 0

namespace Monky
{
	class Timer;

	class Actor : public Object
	{
	public:
		Actor( const std::string& id, Mesh* mesh = nullptr );

		virtual void update( double deltaTime );

		bool isDead() const { return m_isDead; }
		void setLifespan( float lifespan );

		int getActorType() const { return m_actorType; }

	protected:
		bool m_isDead;
		float m_lifespan;
		float m_age;
		int m_actorType;

		static int sm_nextActorID;
	};
}