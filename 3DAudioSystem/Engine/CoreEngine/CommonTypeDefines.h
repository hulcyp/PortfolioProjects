#pragma once

#include "Matrix3.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "AABB2.h"
#include "MatrixStack.h"

namespace Monky
{
	typedef Vector2<int> vec2i;
	typedef Vector2<float> vec2f;
	typedef Vector2<unsigned int> vec2ui;	
	typedef Vector3<int> vec3i;
	typedef Vector3< unsigned int > vec3ui;
	typedef Vector3<float> vec3f;
	typedef Vector4<float> vec4f;
	typedef Matrix3<float> mat3f;
	typedef Matrix4<float> mat4f;
	typedef Quaternion<float> quatf;
	typedef AABB2<float> aabb2f;
	typedef AABB2<int> aabb2i;
	typedef MatrixStack<float> matStackf;

	typedef unsigned char byte;
}