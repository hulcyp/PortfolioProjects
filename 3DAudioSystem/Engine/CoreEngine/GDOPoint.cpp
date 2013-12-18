#include "GDOPoint.h"
#include "Mesh.h"
#include "MeshFactory.h"

namespace Monky
{
	GDOPoint::GDOPoint( float radius, const vec3f& pos, float showUntilTime, Color4f color )
		:	GraphicDebugObject( pos, mat3f::identity(), showUntilTime )
	{
		m_mesh = MeshFactory::generatePointMesh( radius, "DebugMaterial", color );
	}
}