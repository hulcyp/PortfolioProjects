#include "P3DCamera.h"

namespace Monky
{
	P3DCamera::P3DCamera( const std::string& id, const vec3f& pos, const mat4f& projMatrix, const mat4f& viewMatrix )
		:	Camera( id, pos, PH_PERSPECTIVE, projMatrix, viewMatrix )
	{}
	//----------------------------------------------
	P3DCamera::~P3DCamera()
	{}
	//----------------------------------------------
	const mat4f& P3DCamera::getViewMatrix()
	{
		mat3f rot = getLocalRotation();
		vec3f trans = getLocalPosition();
		
		mat4f invTrans( 1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						-trans.x, -trans.y, -trans.z, 1 );
		
		mat3f Rx( mat3f::IDENTITY ), Ry( mat3f::IDENTITY ), Rz( mat3f::IDENTITY ), Rz180( mat3f::IDENTITY );
		Rx.rotate( vec3f::UNIT_X, -90.0f );
		Ry.rotate( vec3f::UNIT_Y, -90.0f );
		Rz.rotate( vec3f::UNIT_Z, -90.0f );
		Rz180.rotate( vec3f::UNIT_Z, 180.0f );

		rot = Rz180 * Ry * Rx * Rz * rot;

		rot.transpose();
		m_viewMatrix = invTrans * rot;
		return m_viewMatrix;
	}
}