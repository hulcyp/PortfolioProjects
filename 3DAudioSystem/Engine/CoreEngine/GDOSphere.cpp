#include "GDOSphere.h"
#include "Mesh.h"
#include "MeshFactory.h"

namespace Monky
{
	GDOSphere::GDOSphere( float radius, int rings, int sectors, const vec3f& pos, float showUntilTime, Color4f color )
		:	GraphicDebugObject( pos, mat3f::identity(), showUntilTime )
	{
		m_mesh = MeshFactory::generateSphere( radius, rings, sectors, "DebugMaterial", color );
	}
}