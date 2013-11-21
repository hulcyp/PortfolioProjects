#include "ASGeometry.h"
#include <CommonEngineIncludes.h>
#include "ASGeometryImpl.h"
#include <Mesh.h>

namespace Monky
{
	std::map< std::string, ASGeometry* > ASGeometry::sm_asGeometries;

	ASGeometry::ASGeometry( Buffer* vertices, VtxBufferAccessor* vtxAccessor, 
			Buffer* indices, BufferAccessor* idxAccessor, 
			float directOcclusion, float reverbOcclusion )
			:	m_pImpl( new ASGeometryImpl( vertices, vtxAccessor, indices, idxAccessor,
				directOcclusion, reverbOcclusion ) )
	{}
	//--------------------------------------------------------------
	ASGeometry::~ASGeometry()
	{
		SAFE_DELETE( m_pImpl );
	}
	//--------------------------------------------------------------
	void ASGeometry::setPosition( const vec3f& pos )
	{
		m_pImpl->setPosition( pos );
	}
	//--------------------------------------------------------------
	vec3f ASGeometry::getPosition() const
	{
		return m_pImpl->getPosition();
	}
	//--------------------------------------------------------------
	void ASGeometry::setOrientation( const mat3f& orientation )
	{
		m_pImpl->setOrientation( orientation );
	}
	//--------------------------------------------------------------
	mat3f ASGeometry::getOrientation() const
	{
		return m_pImpl->getOrientation();
	}
	//--------------------------------------------------------------
	void ASGeometry::setScale( const vec3f& scale )
	{
		m_pImpl->setScale( scale );
	}
	//--------------------------------------------------------------
	vec3f ASGeometry::getScale() const
	{
		return m_pImpl->getScale();
	}
	//--------------------------------------------------------------
	ASGeometry* ASGeometry::createOrGetASGeometry( const std::string& name, Mesh* mesh, float directOcclusion, float reverbOcclusion )
	{
		return createOrGetASGeometry(	name, 
										reinterpret_cast<Buffer*>( mesh->getTriangleBatch( 0 )->vboBuffer ), 
										mesh->getTriangleBatch( 0 )->vboAccessor, 
										reinterpret_cast<Buffer*>( mesh->getTriangleBatch( 0 )->iboBuffer ), 
										mesh->getTriangleBatch( 0 )->iboAccessor, 
										directOcclusion, reverbOcclusion );
	}
	//--------------------------------------------------------------
	ASGeometry* ASGeometry::createOrGetASGeometry( const std::string& name,
											  Buffer* vertices, VtxBufferAccessor* vtxAccessor, 
											  Buffer* indices, BufferAccessor* idxAccessor, float directOcclusion, float reverbOcclusion )
	{
		ASGeometry*& geom = sm_asGeometries[ name ];
		if( geom == nullptr )
		{
			geom = new ASGeometry( vertices, vtxAccessor, indices, idxAccessor, directOcclusion, reverbOcclusion );
		}
		return geom;
	}
	//--------------------------------------------------------------
	void ASGeometry::createASGeometry( const std::string& name, Mesh* mesh, float directOcclusion, float reverbOcclusion )
	{
		createOrGetASGeometry( name, mesh, directOcclusion, reverbOcclusion );
	}
	//--------------------------------------------------------------
	void ASGeometry::createASGeometry( const std::string& name,
											  Buffer* vertices, VtxBufferAccessor* vtxAccessor, 
											  Buffer* indices, BufferAccessor* idxAccessor, float directOcclusion, float reverbOcclusion )
	{
		createOrGetASGeometry( name, vertices, vtxAccessor, indices, idxAccessor, directOcclusion, reverbOcclusion );
	}
	//--------------------------------------------------------------
	ASGeometry* ASGeometry::getASGeometry( const std::string& name )
	{
		ASGeometry* geom = nullptr;
		auto iter = sm_asGeometries.find( name );
		if( iter != sm_asGeometries.end() )
			geom = iter->second;
		return geom;
	}
	//--------------------------------------------------------------
	//	Event functions used to access the as geometry system without needing to know about it... super sneaky lol
	//--------------------------------------------------------------	
	void registerASGeometryEvents()
	{
		static bool isInitialized = false;
		
		if( !isInitialized )
		{
			registerForEvent( "createASGeometry", createASGeometryEvt );
			registerForEvent( "setPosOfASGeometry", setPosOfASGeometryEvt );
			registerForEvent( "getPosOfASGeometry", getPosOfASGeometryEvt );
			registerForEvent( "setOrientationOfASGeometry", setOrientationOfASGeometryEvt );
			registerForEvent( "getOrientationOfASGeometry", getOrientationOfASGeometryEvt );
			registerForEvent( "setScaleOfASGeometry", setScaleOfASGeometryEvt );
			registerForEvent( "getScaleOfASGeometry", getScaleOfASGeometryEvt );
			isInitialized = true;
		}
	}
	//--------------------------------------------------------------	
	void createASGeometryEvt( NamedProperties& params )
	{
		Mesh* mesh = nullptr;
		std::string name = "";
		float directOcclusion = 0.0f;
		float reverbOcclusion = 0.0f;

		if( params.get( "name", name ) != NamedProperties::SUCCESS )
		{
			consolePrintColorf( "No name specified for AS geometry creation. Failed to create ASgeometry", color::RED );
			return;
		}
		
		params.get( "directOcclusion", directOcclusion );
		params.get( "reverbOcclusion", reverbOcclusion );

		if( params.get( "mesh", mesh ) == NamedProperties::SUCCESS )
		{
			ASGeometry::createASGeometry( name, mesh, directOcclusion, reverbOcclusion );
		}
		else
		{
			Buffer* vertices = nullptr;
			Buffer* indices = nullptr;
			VtxBufferAccessor* vtxAccessor = nullptr;
			BufferAccessor* idxAccessor = nullptr;
			if( params.get( "vertices", vertices ) == NamedProperties::SUCCESS	&&
				params.get( "indices", indices ) == NamedProperties::SUCCESS	&&
				params.get( "vtxAccessor", vtxAccessor ) == NamedProperties::SUCCESS &&
				params.get( "idxAccessor", idxAccessor ) == NamedProperties::SUCCESS )
			{
				ASGeometry::createASGeometry( name, vertices, vtxAccessor, indices, idxAccessor, directOcclusion, reverbOcclusion );
			}
			else
			{
				consolePrintColorf( "Not enough geometry data provided to create AS geometry", color::RED );
			}
		}
	}
	//--------------------------------------------------------------	
	void setPosOfASGeometryEvt( NamedProperties& params )
	{
		std::string name;
		vec3f pos;

		if( params.get( "name", name ) != NamedProperties::SUCCESS )
		{
			consolePrintColorf( "No name specified for AS geometry. Failed to retrieve set position", color::RED );
			return;
		}
		ASGeometry* geom = ASGeometry::getASGeometry( name );
		if( geom != nullptr )
		{
			if( params.get( "pos", pos ) == NamedProperties::SUCCESS )
			{
				geom->setPosition( pos );
			}
		}		
	}
	//--------------------------------------------------------------	
	void getPosOfASGeometryEvt( NamedProperties& params )
	{
		std::string name;
		if( params.get( "name", name ) != NamedProperties::SUCCESS )
		{
			consolePrintColorf( "No name specified for AS geometry. Failed to retrieve position", color::RED );
			return;
		}
		ASGeometry* geom = ASGeometry::getASGeometry( name );
		if( geom != nullptr )
		{
			vec3f pos = geom->getPosition();
			params.set( "pos", pos );
		}
	}
	//--------------------------------------------------------------	
	void setOrientationOfASGeometryEvt( NamedProperties& params )
	{
		std::string name;
		mat3f orientation;
		if( params.get( "name", name ) != NamedProperties::SUCCESS )
		{
			consolePrintColorf( "No name specified for AS geometry. Failed to set orientation", color::RED );
			return;
		}
		ASGeometry* geom = ASGeometry::getASGeometry( name );
		if( geom != nullptr )
		{
			if( params.get( "orientation", orientation ) == NamedProperties::SUCCESS )
				geom->setOrientation( orientation );
		}
	}
	//--------------------------------------------------------------	
	void getOrientationOfASGeometryEvt( NamedProperties& params )
	{
		std::string name;
		mat3f orientation;
		if( params.get( "name", name ) != NamedProperties::SUCCESS )
		{
			consolePrintColorf( "No name specified for AS geometry. Failed to retrieve orientation.", color::RED );
			return;
		}
		ASGeometry* geom = ASGeometry::getASGeometry( name );
		if( geom != nullptr )
		{
			if( params.get( "orientation", orientation ) == NamedProperties::SUCCESS )
				geom->setOrientation( orientation );
		}
	}
	//--------------------------------------------------------------	
	void setScaleOfASGeometryEvt( NamedProperties& params )
	{
		std::string name;
		vec3f scale;
		if( params.get( "name", name ) != NamedProperties::SUCCESS )
		{
			consolePrintColorf( "No name specified for AS geometry. Failed to set scale", color::RED );
			return;
		}
		ASGeometry* geom = ASGeometry::getASGeometry( name );
		if( geom != nullptr )
		{
			if( params.get( "scale", scale ) == NamedProperties::SUCCESS )
				geom->setScale( scale );
		}
	}
	//--------------------------------------------------------------	
	void getScaleOfASGeometryEvt( NamedProperties& params )
	{
		std::string name;
		vec3f scale;
		if( params.get( "name", name ) != NamedProperties::SUCCESS )
		{
			consolePrintColorf( "No name specified for AS geometry. Failed to retrieve scale.", color::RED );
			return;
		}
		ASGeometry* geom = ASGeometry::getASGeometry( name );
		if( geom != nullptr )
		{
			if( params.get( "scale", scale ) == NamedProperties::SUCCESS )
				geom->setScale( scale );
		}
	}
}