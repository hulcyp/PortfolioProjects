#include "ProfileData.h"
#include "CommonEngineIncludes.h"
#include "TimeUtils.h"

namespace Monky
{
	typedef std::pair< std::string, Color4f > ColoredString;

	ProfileData::ProfileData( const std::string& profileTag, int index, int parentIndex )
		:	m_profileTag( profileTag )
		,	m_index( index )
		,	m_parentIndex( parentIndex )
		,	m_totalTimeSeconds( 0.0 )
		,	m_timePerFrameSeconds( 0.0 )
		,	m_avgTimePerFrame( 0.0 )
		,	m_totalCount( 0 )
		,	m_countPerFrame( 0 )
		,	m_avgCountPerFrame( 0.0f )
		,	m_extraTotalData( 0 )
		,	m_extraDataPerFrame( 0 )
		,	m_shouldRenderData( true )
		,	m_numberOfItemsToRender( 9 )
		,	m_avgExtraDataPerFrame( 0 )
		,	m_bitFlagsForWhatToRender( 0 )
	{
		for( int i = 0; i < (int)DATA_TO_RENDER_COUNT; ++i )
		{
			m_bitFlagsForWhatToRender = m_bitFlagsForWhatToRender | ( 1 << ( TOTAL_TIME + i ) );
		}
		if( m_parentIndex != -1 )
		{
			ProfileData& profileData = ProfileSystem::getInstance()->getProfileData( parentIndex );
			profileData.m_childIndices.push_back( m_index );
		}

		std::vector< std::string > tokens;
		stringTokenizer( m_profileTag, tokens, "." );
		m_readableTag = tokens[ tokens.size() - 1 ];
	}
	//--------------------------------------------------------------
	void ProfileData::renderData( NamedProperties& fontParams, 
		bool hasMaxTime, bool hasMaxTimePerFrame, bool hasMaxCount, bool hasMaxCountPerFrame,
		bool hasMaxExtraData, bool hasMaxExtraDataPerFrame )
	{
		float fontHeight = 0.0f;
		fontParams.get( "fontHeight", fontHeight );
		vec3f pos( 0.0f, 0.0f, 0.0f );
		fontParams.get( "pos", pos );

		// Update avg values
		m_avgTimePerFrame *= 0.9f;
		m_avgTimePerFrame += 0.1f * m_timePerFrameSeconds;
		m_avgCountPerFrame *= 0.9f;
		m_avgCountPerFrame += 0.1f * m_countPerFrame;

		std::vector< ColoredString > stringsToRender;
		stringsToRender.reserve(  m_numberOfItemsToRender  );

		bool hasParent = m_parentIndex != -1;
		float totalTimePercentage = 0.0f;
		float timePerFramePercentage = 0.0f;
		//float totalCountPercentage;	
		//float countPerFramePercentage;
		if( hasParent )
		{
			const ProfileData& parentData = ProfileSystem::getInstance()->getProfileData( m_parentIndex );
			if( parentData.m_totalTimeSeconds > 0.0 )
				totalTimePercentage		= 100.0f * (float)( m_totalTimeSeconds / parentData.m_totalTimeSeconds );
			if( parentData.m_timePerFrameSeconds > 0.0 )
				timePerFramePercentage	= 100.0f * (float)( m_timePerFrameSeconds / parentData.m_timePerFrameSeconds );
			//totalCountPercentage	= 100.0f * m_totalCount / parentData.m_totalCount;
			//countPerFramePercentage	= 100.0f * m_countPerFrame / parentData.m_countPerFrame;
		}

		if( ( m_bitFlagsForWhatToRender >> TOTAL_TIME ) & 1 )
		{
			stringsToRender.push_back( 
				ColoredString( "Total Time(ms): " + doubleToString( TimeUtils::ConvertTimeFromSecondsTo( m_totalTimeSeconds, TimeUtils::MILLI ), 3 ), 
				color::WHITE ) );
			if( hasMaxTime )
				stringsToRender[ stringsToRender.size() - 1 ].second = color::RED; 
			if( hasParent )
				stringsToRender[ stringsToRender.size() - 1 ].first += "(" + floatToString( totalTimePercentage, 2 ) + "%)";
		}

		if( ( m_bitFlagsForWhatToRender >> TIME_PER_FRAME ) & 1 )
		{
			stringsToRender.push_back( 
				ColoredString( "Time(ms)/Frame: " + doubleToString( TimeUtils::ConvertTimeFromSecondsTo( m_timePerFrameSeconds, TimeUtils::MILLI ), 3 ), 
				color::WHITE ) );
			if( hasMaxTimePerFrame )
				stringsToRender[ stringsToRender.size() - 1 ].second = color::RED; 
			if( hasParent )
				stringsToRender[ stringsToRender.size() - 1 ].first += "(" + floatToString( timePerFramePercentage, 2 ) + "%)";
		}

		if( ( m_bitFlagsForWhatToRender >> AVG_TIME ) & 1 )
			stringsToRender.push_back( ColoredString( "Avg Time(ms)/Frame: " + doubleToString( TimeUtils::ConvertTimeFromSecondsTo( m_avgTimePerFrame, TimeUtils::MILLI ), 3 ), color::WHITE ) );
		
		if( ( m_bitFlagsForWhatToRender >> TOTAL_COUNT ) & 1 )
		{
			stringsToRender.push_back( 
				ColoredString( "Total Count: " + toString( m_totalCount ), 
				color::WHITE ) );
			if( hasMaxTimePerFrame )
				stringsToRender[ stringsToRender.size() - 1 ].second = color::RED; 
			//if( hasParent )
			//	stringsToRender[ stringsToRender.size() - 1 ].first += "(" + floatToString( totalCountPercentage, 2 ) + "%)";
		}

		if( ( m_bitFlagsForWhatToRender >> COUNT_PER_FRAME ) & 1 )
		{
			stringsToRender.push_back( 
				ColoredString( "Count/Frame: " + toString( m_countPerFrame ), 
				color::WHITE ) );
			if( hasMaxTimePerFrame )
				stringsToRender[ stringsToRender.size() - 1 ].second = color::RED; 
			//if( hasParent )
			//	stringsToRender[ stringsToRender.size() - 1 ].first += "(" + floatToString( countPerFramePercentage, 2 ) + "%)";
		}
		
		if( ( m_bitFlagsForWhatToRender >> AVG_COUNT ) & 1 )
			stringsToRender.push_back( ColoredString( "Avg Count/Frame: " + floatToString( m_avgCountPerFrame, 2 ), color::WHITE ) );

		if( ( m_bitFlagsForWhatToRender >> TOTAL_EXTRA_DATA ) & 1 )
		{
			stringsToRender.push_back(
				ColoredString( "Total Extra Data: " + toString( m_extraTotalData ), 
					color::WHITE ) );
			if( hasMaxTimePerFrame )
				stringsToRender[ stringsToRender.size() - 1 ].second = color::RED; 
		}

		if( ( m_bitFlagsForWhatToRender >> EXTRA_DATA_PER_FRAME ) & 1 )
		{
			stringsToRender.push_back(
				ColoredString( "Extra Data/Frame: " + toString( m_extraDataPerFrame ), 
					color::WHITE ) );
			if( hasMaxTimePerFrame )
				stringsToRender[ stringsToRender.size() - 1 ].second = color::RED;
		}

		if( ( m_bitFlagsForWhatToRender >> AVG_EXTRA_DATA ) & 1 )
			stringsToRender.push_back( ColoredString( "Avg Data/Frame: " + floatToString( m_avgExtraDataPerFrame, 2 ), color::WHITE ) );

		fontParams.set( "text", m_readableTag + std::string( ":" ) );
		fontParams.set( "color", color::GREEN );
		mat4f modelMatrix( mat4f::IDENTITY );
		modelMatrix.translate( pos );
		fontParams.set( "modelMatrix", modelMatrix );
		fireEvent( "renderText", fontParams );
		pos.y -= fontHeight;
		pos.x += 10.0f;

		std::stringstream ss;
		for( size_t i = 0; i < stringsToRender.size(); ++i )
		{
			char color[10];
			sprintf( color, "%x", stringsToRender[i].second.getColorAsLong() );
			ss << "<#" << color << ">" << stringsToRender[i].first << '\n';
		}

		//for( size_t i = 0; i < stringsToRender.size(); ++i )
		//{
		//	fontParams.set( "text", stringsToRender[i].first );
		//	fontParams.set( "color", stringsToRender[i].second );
		//	fontParams.set( "pos", pos );
		//	fireEvent( "renderText", fontParams );
		//	pos.y -= fontHeight;
		//}
		fontParams.set( "text", ss.str() );
		fontParams.set( "color", color::WHITE );
		modelMatrix.loadIdentity();
		modelMatrix.translate( pos );
		fontParams.set( "modelMatrix", modelMatrix );
		fireEvent( "renderText", fontParams );

	}
	//--------------------------------------------------------------
	void ProfileData::addData( double deltaTime, int extraData )
	{
		m_totalTimeSeconds += deltaTime;
		m_timePerFrameSeconds += deltaTime;
		m_totalCount++;
		m_countPerFrame++;
		m_extraTotalData += extraData;
		m_extraDataPerFrame += extraData;
		m_avgExtraDataPerFrame *=0.9f;
		m_avgExtraDataPerFrame += 0.1f * extraData;
		ProfileSystem::getInstance()->reportNewData( m_index );
	}
	//--------------------------------------------------------------
	void ProfileData::clearFrameData()
	{
		m_timePerFrameSeconds = 0.0;
		m_countPerFrame = 0;
		m_extraDataPerFrame = 0;
	}
	//--------------------------------------------------------------
	void ProfileData::setShouldRender( bool shouldRender )
	{
		m_shouldRenderData = shouldRender;
	}
	//--------------------------------------------------------------
	void ProfileData::enableDataForRender( DataToRender whatToEnable )
	{
		m_bitFlagsForWhatToRender = m_bitFlagsForWhatToRender | ( 1 << whatToEnable );
		updateNumberOfItemsToRender();		
	}
	//--------------------------------------------------------------
	void ProfileData::disableDataForRender( DataToRender whatToDisable )
	{
		m_bitFlagsForWhatToRender = m_bitFlagsForWhatToRender & ~( 1 << whatToDisable );
		updateNumberOfItemsToRender();
	}
	//--------------------------------------------------------------
	void ProfileData::toggleDataRenderState( DataToRender whatToToggle )
	{
		m_bitFlagsForWhatToRender = m_bitFlagsForWhatToRender ^ 1 << whatToToggle;
		updateNumberOfItemsToRender();
	}
	//--------------------------------------------------------------
	void ProfileData::updateNumberOfItemsToRender()
	{
		m_numberOfItemsToRender = 0;
		for( int i = 0; i < (int)DATA_TO_RENDER_COUNT; ++i )
		{
			if( ( m_bitFlagsForWhatToRender >> ( TOTAL_TIME + i ) ) & 1 )
				++m_numberOfItemsToRender;
		}
	}
	//--------------------------------------------------------------
	bool ProfileData::hasParent() const
	{
		return m_parentIndex != -1;
	}
	//--------------------------------------------------------------
	bool ProfileData::hasChildren() const
	{
		return !m_childIndices.empty();
	}
	//--------------------------------------------------------------
	const std::vector< int >& ProfileData::getChildren() const
	{
		return m_childIndices;
	}
}