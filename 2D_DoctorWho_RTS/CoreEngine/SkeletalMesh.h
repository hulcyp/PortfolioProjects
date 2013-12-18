#pragma once
#include "Mesh.h"
#include "CommonEngineIncludes.h"
#include <Timer.h>
#include "EventSystemInterface.h"

namespace pdh
{
	class SkeletalMesh : public Mesh
	{
	public:
		
		SkeletalMesh( GLBuffer* vboBuffer, GLBuffer* iboBuffer, VtxBufferAccessor* vtxAccessor, const std::string& material, bool rvsWinding = false );
		
		virtual void updateMaterial( const matStackf& modelStack, int index );

		void setFrameRate( float frameRate ) { m_frameRate = frameRate; }
		//void setWorldToBoneMatrix( const mat4f& worldToBone ) { m_worldToBoneMatrix = worldToBone; }
		void addBone( const std::vector< mat4f >& bone );

		void play();
		void pause();
		void stop();

		void createBoneGDOS( NamedProperties& params );
		

		void playEvt( NamedProperties& params );
		void pauseEvt( NamedProperties& params );
		void stopEvt( NamedProperties& params );
		
	private:
		std::vector< mat4f > m_worldToBoneMatrices;
		std::vector< std::vector< mat4f > > m_boneToWorldFrames;

		int m_currentFrame;
		float m_frameRate;
		bool m_isPlaying;
		Timer m_frameTimer;

		static int meshNum;
	};

}