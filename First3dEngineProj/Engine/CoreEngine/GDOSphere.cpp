#include "GDOSphere.h"
#include "Mesh.h"

namespace pdh
{
	GDOSphere::GDOSphere( float radius, int rings, int sectors, const vec3f& pos, float showUntilTime, Color4f color )
		:	GraphicDebugObject( pos, mat4f::identity(), showUntilTime )
	{
		m_mesh = Mesh::generateSphere( radius, rings, sectors, "DebugMaterial", color );
	}
}