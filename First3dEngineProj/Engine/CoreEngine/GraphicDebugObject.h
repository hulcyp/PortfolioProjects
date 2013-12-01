#pragma once
#include "CommonEngineIncludes.h"

namespace pdh
{
	class Mesh;
	class Renderer;

	class GraphicDebugObject
	{
	public:
		GraphicDebugObject( const vec3f& pos, const mat4f& rotation = mat4f::identity(), float showUntilTIme = 0.0f );
		virtual ~GraphicDebugObject();

		float getShowUntilTime() { return m_showUntilTime; }
		void setIsDead( bool isDead ) { m_isDead = isDead; }
		bool isDead() { return m_isDead; }

		virtual void render( Renderer* renderer, float currentTime );

	protected:
		vec3f m_pos;
		mat4f m_rotation;
		Mesh* m_mesh;
		float m_showUntilTime;
		bool m_isDead;
	};
}