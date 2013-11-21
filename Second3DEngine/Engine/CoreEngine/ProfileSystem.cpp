#include "ProfileSystem.h"
#include "Colors.h"

namespace Monky
{
	ProfileSystem g_profileSystem;

#pragma warning( disable:4355 )
	ProfileSystem::ProfileSystem()
		:	Singleton( this )
		,	m_fontHeight( 12.0f )
		,	m_childTabWidth( 10.0f )
		,	m_indexForProfileWithMaxTimeThisFrame( 0 )
		,	m_indexForProfileWithMaxOverallTime( 0 )
		,	m_indexForProfileWithMaxHitCountThisFrame( 0 )
		,	m_indexForProfileWithMaxHitCountOverall( 0 )
		,	m_indexForProfileWithMaxExtraData( 0 )
		,	m_indexForProfileWithMaxExtraDataThisFrame( 0 )
	{
		m_fontParams.set( "fontName", std::string( "mainFont_72" ) );
		m_fontParams.set( "fontHeight", m_fontHeight );
		m_fontParams.set( "color", color::WHITE );
		m_fontParams.set( "posRel", std::string( "TOPLEFT" ) );		
	}
#pragma warning( default:4355 )
	//----------------------------------------
	ProfileSystem::~ProfileSystem()
	{}
	//----------------------------------------
	const ProfileData& ProfileSystem::getProfileData( size_t index ) const
	{
		assertion( 0 <= index && index < m_profilingData.size(), "Profiling index: %d out of range", index );
		return m_profilingData[index];
	}
	//----------------------------------------
	ProfileData& ProfileSystem::getProfileData( size_t index )
	{
		assertion( 0 <= index && index < m_profilingData.size(), "Profiling index: %d out of range", index );
		return m_profilingData[index];
	}
	//----------------------------------------
	int ProfileSystem::getIndexMapping( const std::string& tag )
	{
		int index = -1;
		auto iter = m_tagToindexMapping.find( tag );
		if( iter == m_tagToindexMapping.end() )
		{
			std::vector< std::string > tokens;
			stringTokenizer( tag, tokens, "." );
			int parentID = -1;
			if( tokens.size() > 1 )
			{
				std::string parentTag;
				for( size_t i = 0; i < tokens.size() - 1; ++i )
				{
					parentTag += tokens[i];
					if( i < tokens.size() - 2 )
						parentTag += ".";
				}
				parentID = getIndexMapping( parentTag );
			}
			index = m_profilingData.size();
			m_profilingData.push_back( ProfileData( tag, index, parentID ) );
			m_tagToindexMapping.insert( std::pair< std::string, int >( tag, index ) );
		}
		else
		{
			index = iter->second;
		}
		return index;
	}
	//----------------------------------------
	void ProfileSystem::renderData( const vec3f& startPos, Camera* camera )
	{
		m_fontParams.set( "camera", camera );
		vec3f pos = startPos;
		m_fontParams.set( "pos", pos );
		for( size_t i = 0; i < m_profilingData.size(); ++i )
		{
			if( m_profilingData[i].shouldRender() && !m_profilingData[i].hasParent() )
			{
				m_profilingData[i].renderData( m_fontParams,	i == m_indexForProfileWithMaxOverallTime,
																i == m_indexForProfileWithMaxTimeThisFrame,
																i == m_indexForProfileWithMaxHitCountOverall,
																i == m_indexForProfileWithMaxHitCountThisFrame,
																i == m_indexForProfileWithMaxExtraData,
																i == m_indexForProfileWithMaxExtraDataThisFrame );
				
				int numItemsToRender = m_profilingData[i].getNumberOfItemsToRender() + 1;
				pos.y -= m_fontHeight * numItemsToRender;
				m_fontParams.set( "pos", pos );
				
				if( m_profilingData[i].hasChildren() )
				{
					renderChildren( i, pos );
				}				
			}
		}
	}
	//----------------------------------------
	void ProfileSystem::clearFrameData()
	{
		for( size_t i = 0; i < m_profilingData.size(); ++i )
		{
			m_profilingData[i].clearFrameData();
		}
	}
	//----------------------------------------
	void ProfileSystem::toggleDataRenderState( ProfileData::DataToRender whatToToggle )
	{
		for( size_t i = 0; i < m_profilingData.size(); ++i )
		{
			m_profilingData[i].toggleDataRenderState( whatToToggle );
		}
	}
	//----------------------------------------
	void ProfileSystem::reportNewData( size_t profileIndex )
	{
		if( m_profilingData[ profileIndex ].m_timePerFrameSeconds > m_profilingData[ m_indexForProfileWithMaxTimeThisFrame ].m_timePerFrameSeconds )
			m_indexForProfileWithMaxTimeThisFrame = profileIndex;
		if( m_profilingData[ profileIndex ].m_totalTimeSeconds > m_profilingData[ m_indexForProfileWithMaxOverallTime ].m_totalTimeSeconds )
			m_indexForProfileWithMaxOverallTime = profileIndex;
		if( m_profilingData[ profileIndex ].m_countPerFrame > m_profilingData[ m_indexForProfileWithMaxHitCountThisFrame ].m_countPerFrame )
			m_indexForProfileWithMaxHitCountThisFrame = profileIndex;
		if( m_profilingData[ profileIndex ].m_totalCount > m_profilingData[ m_indexForProfileWithMaxHitCountOverall ].m_totalCount )
			m_indexForProfileWithMaxHitCountOverall = profileIndex;
		if( m_profilingData[ profileIndex ].m_extraTotalData > m_profilingData[ m_indexForProfileWithMaxExtraData ].m_extraTotalData )
			m_indexForProfileWithMaxExtraData = profileIndex;
		if( m_profilingData[ profileIndex ].m_extraDataPerFrame > m_profilingData[ m_indexForProfileWithMaxExtraDataThisFrame ].m_extraDataPerFrame )
			m_indexForProfileWithMaxExtraDataThisFrame = profileIndex;
	}
	//----------------------------------------
	void ProfileSystem::renderChildren( int index, vec3f& pos )
	{
		const std::vector< int >& children = m_profilingData[index].getChildren();
		pos.x += m_childTabWidth;
		m_fontParams.set( "pos", pos );
		for( size_t child = 0; child < children.size(); ++child )
		{
			m_profilingData[ children[ child ] ].renderData( m_fontParams,	children[ child ] == m_indexForProfileWithMaxOverallTime,
																			children[ child ] == m_indexForProfileWithMaxTimeThisFrame,
																			children[ child ] == m_indexForProfileWithMaxHitCountOverall,
																			children[ child ] == m_indexForProfileWithMaxHitCountThisFrame,
																			children[ child ] == m_indexForProfileWithMaxExtraData,
																			children[ child ] == m_indexForProfileWithMaxExtraDataThisFrame );
			
			int numItemsToRender = m_profilingData[ children[ child ] ].getNumberOfItemsToRender() + 1;
			pos.y -= m_fontHeight * numItemsToRender;
			m_fontParams.set( "pos", pos );
			if( m_profilingData[ children[ child ] ].hasChildren() )
			{
				renderChildren( children[ child ], pos );
			}
		}
		pos.x -= m_childTabWidth;
		m_fontParams.set( "pos", pos );
	}
}