#include "Object.h"
#include "Renderer.h"
#include "BoundingSphere.h"
#include "CommonEngineIncludes.h"


namespace Monky
{
	Object::Object( const std::string& id, Mesh* mesh )
		:	Node( id )
		,	m_mesh( mesh )
	{
		if( mesh != nullptr )
		{
			Material* mat = Material::getMaterial( m_mesh->getMaterial() );
			m_occlusionGeometryName = id + "asGeom";
			NamedProperties params;
			params.set( "name", m_occlusionGeometryName );
			params.set( "mesh", m_mesh );
			params.set( "directOcclusion", mat->getDirectOcclusion() );
			params.set( "reverbOcclusion", mat->getReverbOcclusion() );
			fireEvent( "createASGeometry", params );
		}
	}
	//-------------------------------------------------------------------
	BoundingSphere* Object::getBoundingSphere()
	{
		if( m_mesh != nullptr )
		{
			return m_mesh->getBoundingSphere();
		}
		else
		{
			return nullptr;
		}
	}
	//-------------------------------------------------------------------
	void Object::render()
	{
		matStackf modelStack;
		modelStack.applyMatrix( getWorldTransformation() );
		NamedProperties params;
		params.set( "modelMatrix", modelStack.top() );
		params.set( "mesh", m_mesh );
		fireEvent( "renderMesh", params );
	}
	//-------------------------------------------------------------------
	void Object::renderDebug()
	{
		matStackf modelStack;
		modelStack.applyMatrix( getWorldTransformation() );
		NamedProperties params;
		params.set( "modelMatrix", modelStack.top() );
		params.set( "mesh", m_mesh );
		fireEvent( "renderDebugMesh", params );
		
		params.set( "length", 5.0f );
		fireEvent( "renderAxis", params );
	}
	//-------------------------------------------------------------------
	void Object::setPosition( const vec3f& pos, int frame )
	{
		Node::setPosition( pos, frame );
		NamedProperties params;
		params.set( "pos", pos );
		params.set( "name", m_occlusionGeometryName );
		fireEvent( "setPosOfASGeometry", params );
	}
	//-------------------------------------------------------------------
	void Object::setRotation( const mat3f& rot, int frame )
	{
		Node::setRotation( rot, frame );
		NamedProperties params;
		params.set( "orientation", rot );
		params.set( "name", m_occlusionGeometryName );
		fireEvent( "setOrientationOfASGeometry", params );
	}
	//-------------------------------------------------------------------
	void Object::setScale( const vec3f& scale, int frame )
	{
		Node::setScale( scale, frame );
		NamedProperties params;
		params.set( "scale", scale );
		params.set( "name", m_occlusionGeometryName );
		fireEvent( "setScaleOfASGeometry", params );
	}
}