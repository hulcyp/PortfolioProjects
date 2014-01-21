// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "MaxExporter.h"

#define C_DLL_EXPORT extern "C" __declspec(dllexport)

HINSTANCE g_instance;


class ExporterClassDesc : public ClassDesc2
{
public:
	int				IsPublic()						{ return TRUE; }
	void*			Create( BOOL loading = FALSE )	{ return new MaxExporter(); }
	const TCHAR*	ClassName()						{ return _T( "MaxExporter" ); }
	SClass_ID		SuperClassID()					{ return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID()						{ return Class_ID(0x33a262ac, 0x6fa25f72); }
	const TCHAR*	Category()						{ return _T( "Exporters" ); }
	const TCHAR*	InternalName()					{ return _T( "MaxExporter" ); }
	HINSTANCE		HInstanc()						{ return g_instance; }
};

BOOL WINAPI DllMain( HINSTANCE inst, ULONG reason, LPVOID )
{
	if ( DLL_PROCESS_ATTACH == reason )
	{
		g_instance = inst;
		DisableThreadLibraryCalls( g_instance );
		return TRUE;
	}

	return TRUE;
}

C_DLL_EXPORT const TCHAR* LibDescription()
{
	return _T("My 3dsmax Exporter");
}

C_DLL_EXPORT int LibNumberClasses()
{
	return 1;
}

C_DLL_EXPORT ClassDesc* LibClassDesc( int i )
{
	switch( i ) 
	{
		case 0:{
			static ExporterClassDesc s_classDesc;
			return &s_classDesc;}
		default:
			return 0;
	}
}

C_DLL_EXPORT ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

C_DLL_EXPORT ULONG CanAutoDefer()
{
	return 1;
}


