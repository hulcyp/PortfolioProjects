#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 8/28/2013
//	Description:
//
//---------------------------------------------
#include "Socket.h"

namespace Monky
{
	class SocketUDPImpl;

	class SocketUDP : public Socket
	{
	public:
		friend SocketUDPImpl;

		SocketUDP( const std::string& hostName, int port );
		SocketUDP( const SocketUDP& copy );
		virtual ~SocketUDP();

		virtual int Send( char* buffer, int bufLen, const std::string& hostName, int port );
		virtual int Recieve( char* buffer, int bufLen, int milliseconds );
		virtual int Send( Packet* packet, const std::string& hostName, int port );
		virtual int Recieve( Packet* packet, int milliseconds );
		virtual bool Bind();
		virtual int ReplyToLastRecieve( char* buffer, int bufLen );
		virtual int ReplyToLastRecieve( Packet* packet );
		virtual void Close();

		void PrintHostLastRecievedFromToConsole();

	private:
		SocketUDPImpl* m_pImpl;
	};
}