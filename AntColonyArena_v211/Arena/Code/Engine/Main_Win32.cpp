//-----------------------------------------------------------------------------------------------
// Main_Win32.cpp
//-----------------------------------------------------------------------------------------------
#include "Main_Win32.hpp"
#include "../Game/TheGame.hpp"


//-----------------------------------------------------------------------------------------------
// Global variables
//
HDC g_displayDeviceContext = nullptr;
HWND g_windowHandle = nullptr;
bool g_isExiting = false;
Vector2 g_clientRectBorderOffset = Vector2::ZERO;
Vector2 g_clientPhysicalSize = Vector2::ZERO;
const std::string g_appName = "Fighting Ants 2.11 :: Squirrel Eiserloh";


//-----------------------------------------------------------------------------------------------
__declspec(noreturn) void FatalError( const std::string& messageTitle, const std::string& messageText )
{
	std::string fullMessageTitle = g_appName + " :: " + messageTitle;
	std::string fullMessageText = messageText + "\n\nThe application will now close.\n";
	MessageBoxA( NULL, fullMessageText.c_str(), fullMessageTitle.c_str(), MB_OK | MB_ICONERROR | MB_TOPMOST );
	__debugbreak();
	exit( -1 );
}


//-----------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowsMessageHandlingProcedure( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	bool wasMessageHandled = theGame->HandleWin32Message( wmMessageCode, wParam, lParam );
	if( !wasMessageHandled )
	{
		return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );
	}
	else
	{
		return 0;
	}
}


//-----------------------------------------------------------------------------------------------
void CreateOpenGLWindow( HINSTANCE applicationInstanceHandle )
{
	const float maxFractionOfDesktopRectToUse = 0.90f;

	// Register our window class
	WNDCLASSEX windowClassDescription;
	memset( &windowClassDescription, 0, sizeof( windowClassDescription ) );
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = (WNDPROC) WindowsMessageHandlingProcedure; // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle( NULL );
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT( "Generic Window Class" );
	RegisterClassEx( &windowClassDescription );

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect( desktopWindowHandle, &desktopRect );
	Vector2 desktopSize( (float)(desktopRect.right - desktopRect.left), (float)(desktopRect.bottom - desktopRect.top) );
	Vector2 clientRectSize( 16.f, 12.f );
	float scale = maxFractionOfDesktopRectToUse * clientRectSize.CalcScaleRequiredToFitInsideBox( desktopSize );
	clientRectSize *= scale;
	Vector2 offset = 0.5f * ( desktopSize - clientRectSize );
	if( offset.x > offset.y )
	{
		offset.x = (desktopSize.x - clientRectSize.x) - offset.y;
	}

	RECT windowRect = { (long) offset.x, (long) offset.y, (long)(offset.x + clientRectSize.x), (long)(offset.y + clientRectSize.y) };
	IntVector2 windowRectMinsBeforeAdjustment( windowRect.left, windowRect.top );
	g_clientPhysicalSize.SetXY( (float)(windowRect.right - windowRect.left), (float)(windowRect.bottom - windowRect.top) );
	AdjustWindowRectEx( &windowRect, windowStyleFlags, FALSE, windowStyleExFlags );
	g_clientRectBorderOffset.SetXY( (float)(windowRectMinsBeforeAdjustment.x - windowRect.left), (float)(windowRectMinsBeforeAdjustment.y - windowRect.top) );

	WCHAR windowTitle[ 1024 ];
	MultiByteToWideChar( GetACP(), 0, g_appName.c_str(), -1, windowTitle, sizeof(windowTitle)/sizeof(windowTitle[0]) );
	g_windowHandle = CreateWindowEx( windowStyleExFlags, windowClassDescription.lpszClassName, windowTitle, windowStyleFlags,
		windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		NULL, NULL, applicationInstanceHandle, NULL );

	ShowWindow( g_windowHandle, SW_SHOW );
	SetForegroundWindow( g_windowHandle );
	SetFocus( g_windowHandle );

	g_displayDeviceContext = GetDC( g_windowHandle );

	HCURSOR cursor = LoadCursor( NULL, IDC_ARROW );
	SetCursor( cursor );

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset( &pixelFormatDescriptor, 0, sizeof( pixelFormatDescriptor ) );
	pixelFormatDescriptor.nSize			= sizeof( pixelFormatDescriptor );
	pixelFormatDescriptor.nVersion		= 1;
	pixelFormatDescriptor.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType	= PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits	= 24;
	pixelFormatDescriptor.cDepthBits	= 32;
	pixelFormatDescriptor.cAccumBits	= 0;
	pixelFormatDescriptor.cStencilBits	= 1;

	int pixelFormatCode = ChoosePixelFormat( g_displayDeviceContext, &pixelFormatDescriptor );
	SetPixelFormat( g_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor );
	HGLRC openGLRenderingContext = wglCreateContext( g_displayDeviceContext );
	wglMakeCurrent( g_displayDeviceContext, openGLRenderingContext );
}


//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int )
{
	theGame = new TheGame;
	CreateOpenGLWindow( applicationInstanceHandle );
	theGame->Startup( commandLineString );

	while( !theGame->IsQuitting() )
	{
		MSG queuedMessage;
		const int maxMessagesToProcessPerGameFrame = 20;
		for( int messageCount = 0; messageCount < maxMessagesToProcessPerGameFrame; ++ messageCount )
		{
			const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
			if( !wasMessagePresent )
			{
				break;
			}

			TranslateMessage( &queuedMessage );
			DispatchMessage( &queuedMessage );
		}

		theGame->RunFrame();
	}
	theGame->Shutdown();

	delete theGame;
	return 0;
}
