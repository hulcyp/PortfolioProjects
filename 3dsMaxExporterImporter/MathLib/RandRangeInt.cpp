#include "RandRangeInt.h"
#include "RandNumGen.h"
#include <Assertion.h>
#include <StringUtils.h>

namespace pdh
{
	RandRangeInt::RandRangeInt()
		:	m_min( 0 )
		,	m_max( 0 )
	{}
	//------------------------------------------------
	RandRangeInt::RandRangeInt( int min, int max )
		:	m_min( min )
		,	m_max( max )
	{
		assertion( min <= max, "Min value: %d is not less than max value: %d", min, max );
	}
	//------------------------------------------------
	RandRangeInt::RandRangeInt( const std::string& str )
	{
		int min = 0;
		int max = 0;
			
		std::vector< std::string > tokens;
		stringTokenizer( str, tokens, "~" );
		if( tokens.size() == 1 )
		{
			int count = 0;
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
		assertion( min <= max, "Min value: %d is not less than max value: %d", min, max );
		m_min = min;
		m_max = max;
	}
	//------------------------------------------------
	RandRangeInt::~RandRangeInt()
	{}
	//------------------------------------------------
	void RandRangeInt::setRange( int min, int max )
	{
		assertion( min <= max, "Min value: %d is not less than max value: %d", min, max );
		m_min = min;
		m_max = max;
	}
	//------------------------------------------------
	int RandRangeInt::evaluate()
	{
		return RandNumGen::randInRangeInt( m_min, m_max );
	}
	//------------------------------------------------
	int RandRangeInt::evaluate( unsigned int seed )
	{
		RandNumGen::seed( seed );
		return evaluate();
	}
}