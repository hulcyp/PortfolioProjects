#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 8/28/2013
//	Description:
//
//---------------------------------------------

namespace Monky
{
	class SockAddr;
	class Packet;

	class Socket
	{
	public:
		Socket(){}
		virtual ~Socket(){}

		bool IsValidSocket() const { return m_isValidSocket; }

		virtual int Send( char* buffer, int bufLen, const std::string& hostName, int port ) = 0;
		virtual int Recieve( char* buffer, int bufLen, int milliseconds ) = 0;
		virtual int Send( Packet* packet, const std::string& hostName, int port ) = 0;
		virtual int Recieve( Packet* packet, int milliseconds ) = 0;

	protected:
		bool m_isValidSocket;
	};
}