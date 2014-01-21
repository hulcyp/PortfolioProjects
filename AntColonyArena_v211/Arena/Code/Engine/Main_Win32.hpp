//-----------------------------------------------------------------------------------------------
// Main_Win32.hpp
//-----------------------------------------------------------------------------------------------
#ifndef __include_Main_Win32__
#define __include_Main_Win32__

#include "Shared.hpp"
#include "Vector2.hpp"


//-----------------------------------------------------------------------------------------------
// Global variables
//
extern HWND g_windowHandle;
extern HDC g_displayDeviceContext;
extern Vector2 g_clientRectBorderOffset;
extern Vector2 g_clientPhysicalSize;
extern bool g_isExiting;
extern const std::string g_appName;


#endif // __include_Main_Win32__
