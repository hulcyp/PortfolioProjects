#include "GDOOrientation.h"
#include "Mesh.h"
#include "MeshFactory.h"


namespace pdh
{
	GDOOrientation::GDOOrientation( float size, const vec3f& pos, const mat3f& rotation, float showUntilTime )
		:	GraphicDebugObject( pos, rotation, showUntilTime )
	{
		m_mesh = MeshFactory::generateAxis( size, "DebugMaterial" );
	}
	GDOOrientation::GDOOrientation( float size, const vec3f& pos, const mat3f& rotation, Color4f& color, float showUntilTime )
		:	GraphicDebugObject( pos, rotation, showUntilTime )
	{
		m_mesh = MeshFactory::generateAxis( size, color, "DebugMaterial" );
	}
}