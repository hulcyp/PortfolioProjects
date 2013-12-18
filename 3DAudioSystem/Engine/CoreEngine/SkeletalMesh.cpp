#include "SkeletalMesh.h"
#include "Renderer.h"
#include "CommonEngineIncludes.h"

namespace Monky
{
	SkeletalMesh::SkeletalMesh( GLBuffer* vboBuffer, GLBuffer* iboBuffer, VtxBufferAccessor* vtxAccessor, const std::string& material, bool rvsWinding )
		:	Mesh( vboBuffer, iboBuffer, vtxAccessor, material, rvsWinding )
		,	m_currentFrame( 0 )
		,	m_frameRate( 0 )
	{
		++meshNum;
		m_meshType = SKELETAL_MESH;
		registerForEvent( "RenderBones" + toString( meshNum ), *this, &SkeletalMesh::createBoneGDOS );

		registerForEvent( "PlaySkel", *this, &SkeletalMesh::playEvt );
		registerForEvent( "PauseSkel", *this, &SkeletalMesh::pauseEvt );
		registerForEvent( "StopSkel", *this, &SkeletalMesh::stopEvt );

	}
	//---------------------------------------------------	
	void SkeletalMesh::updateMaterial( const matStackf& modelStack, int index )
	{
		if( m_boneToWorldFrames.size() > 0 )
		{
			if( m_isPlaying )
			{
				if( m_frameTimer.isTimeUp() )
				{
					m_currentFrame = MathFuncs<int>::wrap( m_currentFrame + 1, 0, m_boneToWorldFrames[0].size() - 1 );

					m_frameTimer.start();
				}
			}
		}

		Material* material = Material::getMaterial( getTriangleBatch( index )->material );

		for( size_t b = 0; b < m_boneToWorldFrames.size(); ++b )
		{
			mat4f modelToWorldViaBoneSpace = m_worldToBoneMatrices[b] * m_boneToWorldFrames[b][m_currentFrame];
			material->addUniform(  ("Bones[" + toString( b ) + "]").c_str(), modelToWorldViaBoneSpace );
		}
	}
	//---------------------------------------------------
	void SkeletalMesh::addBone( const std::vector< mat4f >& bone )
	{
		if( bone.size() > 0 )
		{
			m_worldToBoneMatrices.push_back( bone[0].inverse() );
			mat4f test = bone[0] * m_worldToBoneMatrices[ m_worldToBoneMatrices.size() - 1 ];
			m_boneToWorldFrames.push_back( bone );
		}

	}
	//---------------------------------------------------
	void SkeletalMesh::play()
	{
		m_isPlaying = true;
		if( !m_frameTimer.wasStarted() )
		{
			m_frameTimer.setTime( m_frameRate );
			m_frameTimer.start();
		}
		else
		{
			m_frameTimer.unPause();
		}
	}
	//-------------------------------------------------------------------
	void SkeletalMesh::pause()
	{
		m_isPlaying = false;
		m_frameTimer.pause();
	}
	//-------------------------------------------------------------------
	void SkeletalMesh::stop()
	{
		m_isPlaying = false;
		m_currentFrame = 0;
		m_frameTimer.stop();
	}
	//-------------------------------------------------------------------
	void SkeletalMesh::createBoneGDOS( NamedProperties& params )
	{		
		for( size_t b = 0; b < m_boneToWorldFrames.size(); ++b )
		{		
			vec3f pos = vec3f( m_boneToWorldFrames[b][m_currentFrame][12], m_boneToWorldFrames[b][m_currentFrame][13], m_boneToWorldFrames[b][m_currentFrame][14] );

			float r = b / (float)m_boneToWorldFrames.size();
			float g = 1 - r;
			
			createOrientationGDO( 2.0f, pos, m_boneToWorldFrames[b][m_currentFrame].getUpper3x3(), Color4f( r, g, 0.0f, 1.0f ), 0.0f );
		}
	}
	//-------------------------------------------------------------------
	void SkeletalMesh::playEvt( NamedProperties& params )
	{
		play();
	}
	//-------------------------------------------------------------------
	void SkeletalMesh::pauseEvt( NamedProperties& params )
	{
		pause();
	}
	//-------------------------------------------------------------------
	void SkeletalMesh::stopEvt( NamedProperties& params )
	{
		stop();
	}
	//-------------------------------------------------------------------
	int SkeletalMesh::meshNum = 0;
}