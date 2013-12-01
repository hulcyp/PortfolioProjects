#pragma once
#include <GameWorld.h>
#include <Material.h>

namespace pdh
{
	struct State
	{
		State( const vec3f& com, const vec3f& vel, const mat4f& rot, const vec3f& angularMom )
			:	CoM( com )
			,	VelCoM( vel )
			,	Rotation( rot )
			,	AngularMomCoM( angularMom )
		{}
		vec3f CoM;
		vec3f VelCoM;
		mat4f Rotation;
		vec3f AngularMomCoM;
	};

	class Mesh;
	class Material;
	class Actor;

	class LightingWorld : public GameWorld
	{
	public:
		LightingWorld( double deltaTime, int width, int height, float fov,  const char* windowTitle );
		virtual ~LightingWorld();

		virtual bool onKeyDown( unsigned char keyCode, int x, int y );
		

	protected:
		virtual void updateSimulation();
		virtual void updateDisplay();
		void updateRigidSimulation();
		mat4f calculateDerivativeOfRot();
		void createLights();
		void updateLightPositions();
		

	private:
		Actor* m_cube;
		vec3f m_lightPos;
		Material* m_material;
		Material* m_fontMaterial;
		Font* m_fieryFont;
		bool m_unlit;
		bool m_renderTangentSpace;
		bool m_enableDiffuse;
		bool m_enableSpecular;
		bool m_enableFog;
		bool m_enableEmissive;
		bool m_enableParallax;

		Material::Light m_lights[ MAX_NUM_LIGHTS ];


		State m_cubeState;

		float m_c;
		float m_k;
		vec3f m_omega;
		vec3f m_force;

		mat4f m_intertiaMatrix;
		vec3f m_moment;
		vec3f m_applyForce;
		vec3f m_springAttachedLoc;
	};
}

