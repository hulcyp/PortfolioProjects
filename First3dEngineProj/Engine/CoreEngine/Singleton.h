#pragma once

/*************************
* author: Preston Hulcy
* last updated: 9-21-12
*************************/

#include "Uncopyable.h"

namespace pdh
{
	template< class T >
	class Singleton : Uncopyable
	{
	public:
		static T* getInstance() 
		{
			return s_instance; 
		}
		static bool doesExist() { return s_instance; }
		static void deleteInstance() { delete s_instance; }

	protected:
		Singleton( T* instance ){ s_instance = instance; }
		~Singleton()
		{}

	private:
		static T* s_instance;
	};

	template< class T>
	T* Singleton<T>::s_instance = nullptr;

}