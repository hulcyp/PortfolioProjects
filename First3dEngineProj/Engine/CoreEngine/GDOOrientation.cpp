#include "GDOOrientation.h"
#include "Mesh.h"


namespace pdh
{
	GDOOrientation::GDOOrientation( float size, const vec3f& pos, const mat4f& rotation, float showUntilTime )
		:	GraphicDebugObject( pos, rotation, showUntilTime )
	{
		m_mesh = Mesh::generateAxis( size, "DebugMaterial" );
	}
}