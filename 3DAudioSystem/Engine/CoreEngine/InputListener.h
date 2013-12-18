#pragma once
#include "MonkyKeys.h"
#include "MonkyMouse.h"

namespace Monky
{
	class InputListener
	{
	public:
		InputListener( bool registerForInput = true );
		virtual ~InputListener();

		void registerForInput();
		void unregisterForInput();

		virtual bool onKeyDown( MonkyKey keyCode, int x, int y ) = 0;
		virtual bool onKeyUp( MonkyKey keyCode, int x, int y ) = 0;
		virtual bool onMouseButton( int mouseKeyCode, MonkyMouseButtonState state, int x, int y ){ return false; }
		virtual void onMouseMove( int x, int y ){}

	private:
		bool m_registeredForInput;
	};
}