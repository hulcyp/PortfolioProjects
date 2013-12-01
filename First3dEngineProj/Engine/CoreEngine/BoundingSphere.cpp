#include "BoundingSphere.h"
#include "Renderer.h"

namespace pdh
{
	BoundingSphere::BoundingSphere( const vec3f& pos, float radius )
		:	m_position( pos )
		,	m_radius( radius )
	{}
	
	BoundingSphere::BoundingSphere( const std::vector< Mesh::Vertex >& vertices )
	{
		if( vertices.size() > 1 )
		{
			genereateBoundingSphere( vertices );
		}
	}

	void BoundingSphere::mergeSpheres( const BoundingSphere& bSphere )
	{
		vec3f dif = m_position - bSphere.m_position;
		float dist = dif.norm();

		if( dist <= m_radius && m_radius > 0.0f )
		{
			// Already inside this bouding sphere
		}
		else if( dist <= bSphere.m_radius && bSphere.m_radius > 0.0f )
		{
			m_position = bSphere.m_position;
			m_radius = bSphere.m_radius;
		}
		else
		{
			float newRadius = ( dist + m_radius + bSphere.m_radius ) * 0.5f;
			vec3f centerDir = dif / dist;

			vec3f newCenter = m_position + centerDir * ( dist * 0.5f );

			m_position = newCenter;
			m_radius = newRadius;
		}

	}

	void BoundingSphere::genereateBoundingSphere( const std::vector< Mesh::Vertex >& vertices )
	{
		vec3f bSpherePos( vertices[0].x, vertices[0].y, vertices[0].z );
		float bSphereRadius = 0.0f;
		for( size_t i = 1; i < vertices.size(); ++i )
		{
			vec3f p2Pos( vertices[i].x, vertices[i].y, vertices[i].z );
			vec3f dif = p2Pos - bSpherePos;
			float dist = dif.norm();

			if( dist <= bSphereRadius )
			{
				//The point is already in the sphere
			}
			else
			{
				float newRadius = ( dist + bSphereRadius ) * 0.5f;

				vec3f centerDir = dif / dist;

				vec3f newCenter = bSpherePos + centerDir * ( dist*0.5f );

				bSpherePos = newCenter;
				bSphereRadius = newRadius;
			}
		}
		m_position = bSpherePos;
		m_radius = bSphereRadius;
	}

	void BoundingSphere::render( Renderer* renderer )
	{
		matStackf matStack = renderer->getModelStack();
		matStack.push();
		matStack.loadIdentity();
		matStack.translate( m_position );
		Mesh* sphere = Mesh::generateSphere( m_radius, 10, 10 );
		renderer->renderMesh( sphere );
		matStack.pop();
	}
}