#include "GDOArrow.h"
#include "GraphicDebugObject.h"
#include "Mesh.h"

namespace pdh
{
	GDOArrow::GDOArrow( const vec3f& dir, float length, const vec3f& pos, float showUntilTime )
		:	GraphicDebugObject( pos, mat4f::identity(), showUntilTime )
	{
		m_mesh = Mesh::generateArrow( dir, length, "DebugMaterial" );
	}
}