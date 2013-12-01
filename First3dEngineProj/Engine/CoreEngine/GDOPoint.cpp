#include "GDOPoint.h"
#include "Mesh.h"

namespace pdh
{
	GDOPoint::GDOPoint( float radius, const vec3f& pos, float showUntilTime, Color4f color )
		:	GraphicDebugObject( pos, mat4f::identity(), showUntilTime )
	{
		m_mesh = Mesh::generatePointMesh( radius, "DebugMaterial", color );
	}
}