#include "GDOArrow.h"
#include "GraphicDebugObject.h"
#include "Mesh.h"
#include "MeshFactory.h"

namespace Monky
{
	GDOArrow::GDOArrow( const vec3f& dir, float length, const vec3f& pos, float showUntilTime )
		:	GraphicDebugObject( pos, mat3f::identity(), showUntilTime )
	{
		m_mesh = MeshFactory::generateArrow( dir, length, "DebugMaterial" );
	}
}