#include "InputSystem.h"
#include "InputListener.h"
#include <algorithm>

namespace Monky
{
	InputSystem g_inputSystem;

#pragma warning( disable:4355 )
	InputSystem::InputSystem()
		:	Singleton( this )
	{}
#pragma warning( default:4355 )
	//------------------------------------------
	InputSystem::~InputSystem()
	{}
	//------------------------------------------
	void InputSystem::registerListener( InputListener* listener )
	{
		m_inputListeners.push_back( listener );
	}
	//------------------------------------------
	void InputSystem::unregisterListener( InputListener* listener )
	{
		m_inputListeners.erase( std::remove_if( m_inputListeners.begin(), m_inputListeners.end(),
			[&]( InputListener* inputListener )
		{
			return inputListener == listener;
		} ), m_inputListeners.end() );
	}
	//------------------------------------------
	void InputSystem::onKeyDown( MonkyKey keyCode, int x, int y )
	{	
		bool used = false;
		for( size_t i = 0; i < m_inputListeners.size(); ++i )
		{
			if( !used )
				used = m_inputListeners[i]->onKeyDown( keyCode, x, y );
		}
	}
	//------------------------------------------
	void InputSystem::onKeyUp( MonkyKey keyCode, int x, int y )
	{
		bool used = false;
		for( size_t i = 0; i < m_inputListeners.size(); ++i )
		{
			if( !used )
				used = m_inputListeners[i]->onKeyUp( keyCode, x, y );
		}
	}
	//------------------------------------------
	void InputSystem::onMouseButton( int mouseKeyCode, MonkyMouseButtonState state, int x, int y )
	{
		bool used = false;
		for( size_t i = 0; i < m_inputListeners.size(); ++i )
		{			
			if( !used )
				used = m_inputListeners[i]->onMouseButton( mouseKeyCode, state, x, y );
		}
	}
	//------------------------------------------
	void InputSystem::onMouseMove( int x, int y )
	{
		for( size_t i = 0; i < m_inputListeners.size(); ++i )
		{			
			m_inputListeners[i]->onMouseMove( x, y );
		}
	}
}