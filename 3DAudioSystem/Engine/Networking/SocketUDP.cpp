#include "CommonEngineIncludes.h"
#include "SocketUDP.h"
#define  WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <ws2tcpip.h>
#include "Packet.h"


#pragma comment( lib, "Ws2_32.lib" )
#pragma comment( lib, "Mswsock.lib" )
#pragma comment( lib, "AdvApi32.lib" )

namespace Monky
{
	class SocketUDPImpl
	{
	public:
		SocketUDPImpl( const std::string& hostName, int port )
			:	m_hostName( hostName )
			,	m_port( port )
			,	m_isValidSocket( true )
		{
			int iResult = 0;
			if( sm_socketCount == 0 )
			{
				iResult = WSAStartup( MAKEWORD( 2, 2 ), &sm_wsaData );
				if( iResult != 0 )
				{
					consolePrintColorf( "WSAStartup failed: %d", color::RED, iResult );
					m_isValidSocket = false;
				}
			}

			if( iResult == 0 )
			{
				ZeroMemory( &m_addrInfo, sizeof( sockaddr_in ) );
				++sm_socketCount;
				m_addrInfo.sin_family = AF_INET;
				m_addrInfo.sin_port = htons( m_port );
				HOSTENT* tempHostName = gethostbyname( m_hostName.c_str() );

				memcpy( &m_addrInfo.sin_addr.s_addr, tempHostName->h_addr_list[0], tempHostName->h_length );

				m_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

				if( m_socket == INVALID_SOCKET )
				{
					consolePrintColorf( "Failed to create socket", color::RED );
					m_isValidSocket = false;
				}
				if( m_isValidSocket )
				{
					m_event = WSACreateEvent();
					iResult = WSAEventSelect( m_socket, m_event, FD_READ | FD_OOB );
					if( iResult != 0 )
					{
						consolePrintColorf( "Failed to select event: %d", color::RED, WSAGetLastError() );
						m_isValidSocket = false;
					}
				}
			}
		}
		//----------------------------------------------------------------------
		~SocketUDPImpl()
		{}
		//----------------------------------------------------------------------
		bool Bind()
		{
			int iResult;
			bool succeeded = true;						
			
			iResult = bind( m_socket, (sockaddr*)( &m_addrInfo ), sizeof( m_addrInfo ) );
			if( iResult == SOCKET_ERROR )
			{
				consolePrintColorf( "Failed to bind socket using: %s:%d", color::RED, m_hostName.c_str(), m_port );
				consolePrintColorf( "Error Code: %d", color::RED, WSAGetLastError() );
				succeeded = false;
				closesocket( m_socket );
				m_socket = INVALID_SOCKET;
			}

			return succeeded;
		}
		//----------------------------------------------------------------------
		int Send( char* buffer, int bufLen, const std::string& hostName, int port )
		{			
			int iResult = 0;
			sockaddr_in dest;
			ZeroMemory( &dest, sizeof( sockaddr_in ) );
			dest.sin_family = AF_INET;
			dest.sin_port = htons( port );
			HOSTENT* tempHostName = gethostbyname( hostName.c_str() );

			memcpy( &dest.sin_addr.s_addr, tempHostName->h_addr_list[0], tempHostName->h_length );

			iResult = sendto( m_socket, buffer, bufLen, 0, (sockaddr*)&dest, sizeof( dest ) );
			if( iResult == SOCKET_ERROR )
			{
				consolePrintColorf( "Failed to send data", color::RED );
				consolePrintColorf( "Error Code: %d", color::RED, WSAGetLastError() );				
			}
			return iResult;
		}
		//----------------------------------------------------------------------
		int Recieve( char* buffer, int bufLen, int milliseconds )
		{
			sockaddr_in from;
			int iResult = -1;
			int fromlen = sizeof( from );

			if( WaitForSingleObject( m_event, milliseconds ) == WAIT_OBJECT_0 )
			{
				iResult = recvfrom( m_socket, buffer, bufLen, 0, (sockaddr*)&from, &fromlen );
				if( iResult == -1 )
				{
					if( WSAGetLastError() != WSAEWOULDBLOCK )
						consolePrintColorf( "Failed to recieve data. Error code: %d", color::RED, WSAGetLastError() );				
				}

				m_lastFrom = from;
			}
			return iResult;
		}
		//----------------------------------------------------------------------
		int Send( Packet* packet, const std::string& hostName, int port )
		{
			return Send( packet->GetBuffer(), packet->GetBufferSize(), hostName, port );
		}
		//----------------------------------------------------------------------
		int Recieve( Packet* packet, int milliseconds )
		{
			return Recieve( packet->GetBuffer(), packet->GetBufferSize(), milliseconds );
		}
		//----------------------------------------------------------------------
		int ReplyToLastRecieve( char* buffer, int bufLen )
		{
			int iResult = sendto( m_socket, buffer, bufLen, 0, (sockaddr*)&m_lastFrom, sizeof( m_lastFrom ) );
			if( iResult == SOCKET_ERROR )
			{
				consolePrintColorf( "Failed to send data to last recieved", color::RED );
				return WSAGetLastError();
			}
			return iResult;
		}
		int ReplyToLastRecieve( Packet* packet )
		{
			return ReplyToLastRecieve( packet->GetBuffer(), packet->GetBufferSize() );
		}
		//----------------------------------------------------------------------
		void PrintHostLastRecievedFromToConsole()
		{
			char hostName[256];
			hostName[0] = '\0';
			//int len = 0;
			getnameinfo( (sockaddr*)&m_lastFrom, sizeof( m_lastFrom ), hostName, 256, nullptr, MAKEWORD(2,2), 0 );
			consolePrintf( "Socket host: %s", hostName );
		}
		//----------------------------------------------------------------------
		SocketUDPImpl* Clone() const
		{
			SocketUDPImpl* copy = new SocketUDPImpl();
			copy->m_socket = m_socket;
			copy->m_addrInfo = m_addrInfo;
			copy->m_hostName = m_hostName;
			copy->m_port = m_port;
			copy->m_lastFrom = m_lastFrom;
			copy->m_event = WSACreateEvent();
			copy->m_event = WSACreateEvent();
			int iResult = WSAEventSelect( copy->m_socket, copy->m_event, FD_READ | FD_OOB );
			if( iResult != 0 )
			{
				consolePrintColorf( "Failed to select event: %d", color::RED, WSAGetLastError() );
				copy->m_isValidSocket = false;
			}
			return copy;
		}
		//----------------------------------------------------------------------
		void Close()
		{
			--sm_socketCount;
			shutdown( m_socket, SD_SEND );
			closesocket( m_socket );	
			m_socket = INVALID_SOCKET;
			WSACloseEvent( m_event );
			if( sm_socketCount == 0 )
				WSACleanup();
		}

		bool IsValidSocket() const { return m_isValidSocket; }

	private:
		SocketUDPImpl()
			:	m_hostName( "" )
			,	m_port( 0 )
			,	m_socket( INVALID_SOCKET )
		{}

		SOCKET m_socket;
		sockaddr_in m_addrInfo;
		std::string m_hostName;
		int m_port;

		sockaddr_in m_lastFrom;
		bool m_isValidSocket;
		WSAEVENT m_event;

		static int sm_socketCount;
		static WSADATA sm_wsaData;
	};

	int SocketUDPImpl::sm_socketCount = 0;
	WSADATA SocketUDPImpl::sm_wsaData;

	//----------------------------------------------------------------------
	SocketUDP::SocketUDP( const std::string& hostName, int port )
	{
		m_pImpl = new SocketUDPImpl( hostName, port );
		m_isValidSocket = m_pImpl->IsValidSocket();
	}
	SocketUDP::SocketUDP( const SocketUDP& copy )
	{
		m_pImpl = copy.m_pImpl->Clone();
	}
	//----------------------------------------------------------------------
	SocketUDP::~SocketUDP()
	{
		SAFE_DELETE( m_pImpl );
	}
	//----------------------------------------------------------------------
	bool SocketUDP::Bind()
	{
		return m_pImpl->Bind();
	}
	//----------------------------------------------------------------------
	int SocketUDP::Send( char* buffer, int bufLen, const std::string& hostName, int port )
	{
		return m_pImpl->Send( buffer, bufLen, hostName, port );
	}
	//----------------------------------------------------------------------
	int SocketUDP::Recieve( char* buffer, int bufLen, int milliseconds )
	{
		return m_pImpl->Recieve( buffer, bufLen, milliseconds );
	}
	//----------------------------------------------------------------------
	int SocketUDP::Send( Packet* packet, const std::string& hostName, int port )
	{
		return m_pImpl->Send( packet, hostName, port );
	}
	//----------------------------------------------------------------------
	int SocketUDP::Recieve( Packet* packet, int milliseconds )
	{
		return m_pImpl->Recieve( packet, milliseconds );
	}
	//----------------------------------------------------------------------
	int SocketUDP::ReplyToLastRecieve( char* buffer, int bufLen )
	{
		return m_pImpl->ReplyToLastRecieve( buffer, bufLen );
	}
	//----------------------------------------------------------------------
	int SocketUDP::ReplyToLastRecieve( Packet* packet )
	{
		return m_pImpl->ReplyToLastRecieve( packet );
	}
	//----------------------------------------------------------------------
	void SocketUDP::Close()
	{
		m_pImpl->Close();
	}
	//----------------------------------------------------------------------
	void SocketUDP::PrintHostLastRecievedFromToConsole()
	{
		m_pImpl->PrintHostLastRecievedFromToConsole();
	}
}