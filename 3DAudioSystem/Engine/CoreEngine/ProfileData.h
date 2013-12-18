#pragma once
#include "CommonTypeDefines.h"
#include "StdLibraryIncludes.h"
#include "NamedProperties.h"

namespace Monky
{
	class ProfileSystem;

	class ProfileData
	{
	public:
		friend ProfileSystem;

		enum DataToRender
		{
			TOTAL_TIME = 0,
			TIME_PER_FRAME,
			AVG_TIME,
			TOTAL_COUNT,
			COUNT_PER_FRAME,
			AVG_COUNT,
			TOTAL_EXTRA_DATA,
			EXTRA_DATA_PER_FRAME,
			AVG_EXTRA_DATA,
			DATA_TO_RENDER_COUNT
		};

		ProfileData( const std::string& profileTag, int index, int parentIndex = -1 );

		void renderData( NamedProperties& fontParams, 
			bool hasMaxTime, bool hasMaxTimePerFrame, bool hasMaxCount, bool hasMaxCountPerFrame,
			bool hasMaxExtraData, bool hasMaxExtraDataPerFrame );
		
		void addData( double deltaTime, int extraData );

		void clearFrameData();
		bool shouldRender() const { return m_shouldRenderData; }
		int getNumberOfItemsToRender() { return m_numberOfItemsToRender; }

		void setShouldRender( bool shouldRender );
		void enableDataForRender( DataToRender whatToEnable );
		void disableDataForRender( DataToRender whatToDisable );
		void toggleDataRenderState( DataToRender whatToToggle );

		bool hasParent() const;
		bool hasChildren() const;
		const std::vector< int >& ProfileData::getChildren() const;
		

	protected:
		void updateNumberOfItemsToRender();

	private:
		std::string m_profileTag;
		std::string m_readableTag;
		int m_index;
		int m_parentIndex;
		std::vector< int > m_childIndices;
		double m_totalTimeSeconds;
		double m_timePerFrameSeconds;
		double m_avgTimePerFrame;
		int m_totalCount;
		int m_countPerFrame;
		float m_avgCountPerFrame;
		int m_extraTotalData;
		int m_extraDataPerFrame;
		float m_avgExtraDataPerFrame;
		bool m_shouldRenderData;
		int m_numberOfItemsToRender;

		int m_bitFlagsForWhatToRender;
	};
}