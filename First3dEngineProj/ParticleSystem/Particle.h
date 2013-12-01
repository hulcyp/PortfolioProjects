#pragma once
#include <StateSpaceNumericalMethods.h>
#include <Vector3.h>
#include <Mesh.h>

namespace pdh
{
	typedef Vector3<float> vec3;

	class Renderer;

	class Particle
	{
	public:
		Particle();
		Particle( vec3 pos, vec3 vel, float mass = 1.0f, float lifespan = 0 );
		virtual ~Particle() {}

		virtual void render( Renderer* renderer );

		void updateState( State< vec3, float > newState );
		State< vec3, float > getState() { return m_currentState; }
		vec3 getPos() { return m_currentState[0]; }
		vec3 getVel() { return m_currentState[1]; }
		float getMass() { return m_mass; }

		void updateLifspan( float deltaTime );
		bool wantsToDie() const { return m_isDead; }

	protected:
		virtual void buildVBO();

	protected:
		State< vec3, float > m_currentState;
		float m_mass;
		float m_age;
		float m_lifespan;
		bool m_isDead;
		std::shared_ptr< Mesh::Vertex > m_mesh;
		GLuint m_idVBO;
	};
}