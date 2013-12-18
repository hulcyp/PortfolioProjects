#pragma once

namespace Monky
{
	class ProfileSection
	{
	public:
		ProfileSection( int index, int extraData = 0 );
		~ProfileSection();

	private:
		int m_index;
		double m_startTime;
		int m_extraData;
	};
}