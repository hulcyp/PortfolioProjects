#include "RandRangeFloat.h"
#include "RandNumGen.h"
#include <Assertion.h>
#include <StringUtils.h>

namespace Monky
{
	RandRangeFloat::RandRangeFloat()
		:	m_min( 0 )
		,	m_max( 0 )
	{}
	//-----------------------------------------------------------
	RandRangeFloat::RandRangeFloat( float min, float max )
		:	m_min( min )
		,	m_max( max )
	{
		assertion( min <= max, "Min value: %f is not less than max value: %f", min, max );
	}
	//------------------------------------------------
	RandRangeFloat::RandRangeFloat( const std::string& str )
	{
		float min = 0;
		float max = 0;
			
		std::vector< std::string > tokens;
		stringTokenizer( str, tokens, "~" );
		if( tokens.size() == 1 )
		{
			float count = 0;
			stringToType( tokens[0], &count );
			min = count;
			max = count;
		}
		else if( tokens.size() == 2 )
		{
			if( !stringToType( tokens[0], &min ) )
			{
				min = 0;
			}
			if( !stringToType( tokens[1], &max ) )
			{
				max = 0;
			}
		}
		assertion( min <= max, "Min value: %f is not less than max value: %f", min, max );
		m_min = min;
		m_max = max;
	}
	//-----------------------------------------------------------
	RandRangeFloat::~RandRangeFloat()
	{}
	//------------------------------------------------
	void RandRangeFloat::setRange( float min, float max )
	{
		assertion( min <= max, "Min value: %d is not less than max value: %d", min, max );
		m_min = min;
		m_max = max;
	}
	//-----------------------------------------------------------
	float RandRangeFloat::evaluate()
	{
		return RandNumGen::randInRangeFloat( m_min, m_max );
	}
	//-----------------------------------------------------------
	float RandRangeFloat::evaluate( unsigned int seed )
	{
		RandNumGen::seed( seed );
		return evaluate();
	}
}