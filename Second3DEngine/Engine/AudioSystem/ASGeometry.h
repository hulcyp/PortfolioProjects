#pragma once
#include <CommonTypeDefines.h>
#include <map>
#include <EventSystemInterface.h>

namespace Monky
{
	class ASGeometryImpl;
	class Buffer;
	class VtxBufferAccessor;
	class BufferAccessor;
	class Mesh;

	class ASGeometry
	{
	public:
		ASGeometry( Buffer* vertices, VtxBufferAccessor* vtxAccessor, 
			Buffer* indices, BufferAccessor* idxAccessor,
			float directOcclusion, float reverbOcclusion );
		~ASGeometry();

		void setPosition( const vec3f& pos );
		vec3f getPosition() const;

		void setOrientation( const mat3f& orientation );
		mat3f getOrientation() const;

		void setScale( const vec3f& scale );
		vec3f getScale() const;

		void enableOcclusion( bool enabled );
		bool isOcclusionEnalbed() const;

		static ASGeometry* createOrGetASGeometry( const std::string& name, Mesh* mesh, float directOcclusion, float reverbOcclusion );
		static ASGeometry* createOrGetASGeometry( const std::string& name,
												  Buffer* vertices, VtxBufferAccessor* vtxAccessor, 
												  Buffer* indices, BufferAccessor* idxAccessor, float directOcclusion, float reverbOcclusion );
		
		static void createASGeometry( const std::string& name, Mesh* mesh, float directOcclusion, float reverbOcclusion );
		static void createASGeometry( const std::string& name,
												  Buffer* vertices, VtxBufferAccessor* vtxAccessor, 
												  Buffer* indices, BufferAccessor* idxAccessor, float directOcclusion, float reverbOcclusion );
		static ASGeometry* getASGeometry( const std::string& name );
		
	private:
		ASGeometryImpl* m_pImpl;

		static std::map< std::string, ASGeometry* > sm_asGeometries;		
	};

	void registerASGeometryEvents();
	void createASGeometryEvt( NamedProperties& params );
	void setPosOfASGeometryEvt( NamedProperties& params );
	void getPosOfASGeometryEvt( NamedProperties& params );
	void setOrientationOfASGeometryEvt( NamedProperties& params );
	void getOrientationOfASGeometryEvt( NamedProperties& params );
	void setScaleOfASGeometryEvt( NamedProperties& params );
	void getScaleOfASGeometryEvt( NamedProperties& params );
}