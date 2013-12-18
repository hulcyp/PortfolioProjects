#pragma once
#include "GraphicDebugObject.h"

namespace pdh
{
	class Actor;

	class GDOText : public GraphicDebugObject
	{
	public:
		GDOText( Actor* actor, const std::string& type, float showUntilTime = 0.0f, Color4f color = color::WHITE, float fontHeight = 1.0f );

		virtual void render( Renderer* renderer, float currentTime );

	private:
		std::string getStringBasedOnType();
	private:
		Actor* m_actor;
		std::string m_type;
		Color4f m_color;
		float m_fontHeight;
	};
}