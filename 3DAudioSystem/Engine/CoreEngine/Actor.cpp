#include "Actor.h"
#include "Renderer.h"
#include "Model.h"
#include "Font.h"

namespace Monky
{
	Actor::Actor( const std::string& id, Mesh* mesh )
		:	Object( id + "_" + toString( sm_nextActorID++ ), mesh )
		,	m_age( 0.0f )
		,	m_lifespan( 0.0f )
		,	m_isDead( false )
		,	m_actorType( DEFAULT_ACTOR_TYPE )
	{}

	void Actor::update( double deltaTime )
	{
		m_age += static_cast<float>( deltaTime );
		if( m_lifespan > 0.0f && m_age > m_lifespan )
			m_isDead = true;
	}

	void Actor::setLifespan( float lifespan )
	{
		m_lifespan = lifespan;
	}

	int Actor::sm_nextActorID = 0;
}