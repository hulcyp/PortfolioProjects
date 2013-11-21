#pragma once

/*************************
* author: Preston Hulcy
* last updated: 9-21-12
*************************/

namespace Monky
{
	class Uncopyable
	{
	protected:
		Uncopyable(){}
		~Uncopyable(){}
		
	private:
		Uncopyable( const Uncopyable& );
		Uncopyable& operator=( const Uncopyable& );
	};
}