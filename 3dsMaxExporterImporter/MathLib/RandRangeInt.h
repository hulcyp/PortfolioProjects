#pragma once
#include <string>

namespace pdh
{
	class RandRangeInt
	{
	public:
		RandRangeInt();
		RandRangeInt( int min, int max );
		RandRangeInt( const std::string& str );
		~RandRangeInt();

		int evaluate();
		int evaluate( unsigned int );
		int getMin() const { return m_min; }
		int getMax() const { return m_max; }

		void setRange( int min, int max );

	public:
		int m_min;
		int m_max;
	};
}