#pragma once
#include "CommonTypeDefines.h"
#include "Colors.h"

namespace Monky
{
	class GDOManager;
	class ActorManager;	

	GDOManager* initGDOManager( ActorManager* aManager );
	void cleanupGDOManager();
	void createPointGDO( float radius, const vec3f& pos, float showUntilTime = 0.0f, Color4f color = color::WHITE );
	void createSphereGDO( float radius, int rings, int sectors, const vec3f& pos, float showUntilTime = 0.0f, Color4f color = color::WHITE );
	void createOrientationGDO( float size, const vec3f& pos, const mat3f& rotation, float showUntilTime = 0.0f );
	void createOrientationGDO( float size, const vec3f& pos, const mat3f& rotation, Color4f color, float showUntilTime = 0.0f );
	void createArrowGDO( const vec3f& dir, float length, const vec3f& pos, float showUntilTime = 0.0f );
	void createActorGDO( const std::string& id, float showUntilTime = 0.0f, Color4f color = color::WHITE );
	void createTextGDO( const std::string& actorID, const std::string& type, float showUntiltime = 0.0f, Color4f color = color::WHITE, float fontHeight = 1.0f );
}