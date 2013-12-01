#pragma once
#include "Colors.h"
#include <stdarg.h>
#include <memory>
#include <string>
#include <Matrix3.h>
#include <Matrix4.h>
#include <Vector3.h>
#include <Vector2.h>
#include <Vector4.h>
#include <Assertion.h>

namespace pdh
{
	typedef Vector2<int> vec2i;
	typedef Vector2<float> vec2f;
	typedef Vector2< unsigned int > vec2ui;	
	typedef Vector3< int > vec3i;
	typedef Vector3< unsigned int > vec3ui;
	typedef Vector3<double> vec3d;
	typedef Vector3<float> vec3f;
	typedef Vector4<float> vec4f;
	typedef Matrix3<float> mat3f;
	typedef Matrix4<double> mat4d;
	typedef Matrix4<float> mat4f;

	class ConsoleSystem;
	class Renderer;
	class GDOManager;
	class ActorManager;

	ConsoleSystem * initConsole( int width, int height, int bufferSize, Renderer* renderer );
	void consolePrintf( const char* text, ... );
	void consolePrintColorf( const char* text, Color4f color, ... );
	void vConsolePrintf( const char* text, Color4f color, va_list args );
	void executeConsoleCommand( const std::string& cmd, const std::string& args );
	
	GDOManager* initGDOManager( ActorManager* aManager );
	void createPointGDO( float radius, const vec3f& pos, float showUntilTime = 0.0f, Color4f color = color::WHITE );
	void createSphereGDO( float radius, int rings, int sectors, const vec3f& pos, float showUntilTime = 0.0f, Color4f color = color::WHITE );
	void createOrientationGDO( float size, const vec3f& pos, const mat4f& rotation, float showUntilTime = 0.0f );
	void createArrowGDO( const vec3f& dir, float length, const vec3f& pos, float showUntilTime = 0.0f );
	void createActorGDO( const std::string& id, float showUntilTime = 0.0f, Color4f color = color::WHITE );
	void createTextGDO( const std::string& actorID, const std::string& type, float showUntiltime = 0.0f, Color4f color = color::WHITE, float fontHeight = 1.0f );
	
}

#define SAFE_DELETE( ptr ) {	\
	if( ptr != nullptr )		\
	{							\
		delete ptr;				\
		ptr = nullptr;			\
	}							\
}

// Cleans up a vector of ptrs
#define VECTOR_CLEANUP( vector ) {				\
	for( size_t i = 0; i < vector.size(); ++i )	\
	{											\
		delete vector[i];						\
	}											\
	vector.clear();								\
}

// Cleans up a map of keys to ptrs
#define MAP_CLEANUP( map ) {									\
	for( auto iter = map.begin(); iter != map.end(); ++iter )	\
	{															\
		SAFE_DELETE( iter->second );							\
	}															\
	map.clear();												\
}

// Cleans up a list of ptrs
#define LIST_CLEANUP( list ) {									\
	for( auto iter = list.begin(); iter != list.end(); ++iter )	\
	{															\
		SAFE_DELETE( *iter );									\
	}															\
	list.clear();												\
}