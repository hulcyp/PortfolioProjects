#include "StringUtils.h"
#include <sstream>

namespace pdh
{
	template< typename T >
	std::string toString( T i )
	{
		std::stringstream out;
		out << i;
		return out.str();
	}
}