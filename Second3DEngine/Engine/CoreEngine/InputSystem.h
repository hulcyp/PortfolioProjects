#pragma once
#include <CommonEngineIncludes.h>
#include <Singleton.h>
#include <MonkyKeys.h>
#include <MonkyMouse.h>

namespace Monky
{
	class InputListener;

	class InputSystem : public Singleton< InputSystem >
	{
	public:
		InputSystem();
		~InputSystem();

		void registerListener( InputListener* listener );
		void unregisterListener( InputListener* listener );

		void onKeyDown( MonkyKey keyCode, int x, int y );
		void onKeyUp( MonkyKey keyCode, int x, int y );
		void onMouseButton( int mouseKeyCode, MonkyMouseButtonState state, int x, int y );
		void onMouseMove( int x, int y );

		void setCurrentModifiers( MonkyMod modifiers ) { m_currentModifiers = modifiers; }
		MonkyMod getCurrentModifiers() const { return m_currentModifiers; }

	private:
		std::vector< InputListener* > m_inputListeners;
		MonkyMod m_currentModifiers;
	};
}