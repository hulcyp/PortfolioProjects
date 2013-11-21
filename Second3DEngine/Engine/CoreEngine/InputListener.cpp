#pragma once
#include "InputListener.h"
#include "InputSystem.h"

namespace Monky
{
	InputListener::InputListener( bool registerForInput )
		:	m_registeredForInput( registerForInput )
	{
		if( registerForInput )
			InputSystem::getInstance()->registerListener( this );
	}
	//------------------------------------------------------
	InputListener::~InputListener()
	{
		if( m_registeredForInput )
			InputSystem::getInstance()->unregisterListener( this );
	}
	//------------------------------------------------------
	void InputListener::registerForInput()
	{
		if( !m_registeredForInput )
		{
			m_registeredForInput = true;
			InputSystem::getInstance()->registerListener( this );
		}
	}
	//------------------------------------------------------
	void InputListener::unregisterForInput()
	{
		if( m_registeredForInput )
		{
			m_registeredForInput = false;
			InputSystem::getInstance()->unregisterListener( this );
		}
	}
}