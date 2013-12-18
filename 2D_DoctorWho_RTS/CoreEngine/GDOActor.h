#pragma once
#include "GraphicDebugObject.h"

namespace pdh
{
	class Actor;
	class Model;

	class GDOActor : public GraphicDebugObject
	{
	public:
		GDOActor( Actor* actor, float showUntilTime, const std::string& materialName, Color4f color = color::BLUE );

		virtual void render( Renderer* renderer, float currentTime );

	private:
		Actor* m_actor;
		Color4f m_color;
		std::string m_materialName;
	};
}