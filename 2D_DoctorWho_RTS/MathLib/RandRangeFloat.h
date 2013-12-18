#pragma once
#include <string>

namespace pdh
{
	class RandRangeFloat
	{
	public:
		RandRangeFloat();
		RandRangeFloat( float min, float max );
		RandRangeFloat( const std::string& str );
		~RandRangeFloat();

		float evaluate();
		float evaluate( unsigned int );
		float getMin() const { return m_min; }
		float getMax() const { return m_max; }

		void setRange( float min, float max );

	public:
		float m_min;
		float m_max;
	};
}